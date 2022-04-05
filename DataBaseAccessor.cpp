#include "DataBaseAccessor.h"

#include <QErrorMessage>
#include <QSqlRecord>
#include <QSqlError>
#include <qDebug>
#include <QFile>
#include <QMessageBox>
#include <QXmlStreamReader>

DataBaseAccessor::DataBaseAccessor()
{
    _db = new QSqlDatabase();
    *_db = QSqlDatabase::addDatabase("QPSQL", "coffee");

    QFile inFile("config.xml");

    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "Unable to open config file! Set default values!";

        _db->setDatabaseName("coffee_management");
        _db->setUserName("postgres");
        _db->setHostName("127.0.0.1");
        _db->setPort(5432);
        _db->setPassword("12345678");

    } else {
        QXmlStreamReader xmlReader(&inFile);
        xmlReader.readNextStartElement();
        QString name;
        QString user;
        QString host;
        QString port;
        QString password;
        while(!xmlReader.atEnd())
        {
            xmlReader.readNext();
            if (!xmlReader.isStartElement()) {
                continue;
            }

            if (xmlReader.name().toString() == "db_name")
                name = xmlReader.readElementText();
            else if (xmlReader.name().toString() == "user")
                user = xmlReader.readElementText();
            else if (xmlReader.name().toString() == "host")
                host = xmlReader.readElementText();
            else if (xmlReader.name().toString() == "port")
                port = xmlReader.readElementText();
            else if (xmlReader.name().toString() == "password")
                password = xmlReader.readElementText();
        }

        _db->setDatabaseName(name);
        _db->setUserName(user);
        _db->setHostName(host);
        _db->setPort(port.toInt());
        _db->setPassword(password);
    }

    _db->open();
    if (!_db->isOpen()) {
        QMessageBox msgBox;
        msgBox.setText("Ошибка при чтении файла конфигурации!");
        if (msgBox.exec())
        {
            exit(0);
        }
    }
}

bool DataBaseAccessor::processRequest(QString uid, QVector<QString> &table_names, QVector<QVector<QString>> &results, QVector<QString> args)
{
    results.clear();
    QSqlQuery query_for_request(*_db);
    if (query_for_request.exec(QString("SELECT sql FROM sql_requests WHERE id = '%1'").arg(uid)) && query_for_request.size() > 0){
        query_for_request.next();
        QSqlQuery query_for_result(*_db);
        QString newSQL = setArgs(QString(query_for_request.value(0).toString()), args);
        if (query_for_result.exec(newSQL)){
            query_for_result.next();
            for (int i = 0; i < query_for_result.record().count(); i++)
                table_names.append(query_for_result.record().fieldName(i));

            do {
                QVector<QString> curResult;
                for (int i = 0; i < query_for_result.record().count(); i++)
                    curResult.append(query_for_result.value(i).toString());

                results.append(curResult);
            } while (query_for_result.next());

            return true;
        } else {
            qCritical() << "Failed executing SQL-query - " << query_for_result.lastError() << '\n' << uid << uid << " - " << newSQL;
        }

    } else {
        qCritical() << "Failed executing SQL-query - " << query_for_request.lastError() << '\n' << uid << uid << " - " << QString("SELECT sql FROM sql_requests WHERE id = '%1'").arg(uid);
    }
    return false;
}

QString DataBaseAccessor::setArgs(QString query, QVector<QString> args)
{
    for (int i = 0; i < args.size(); i++) {
        query.replace(QString("{$%1}").arg(i+1), args[i]);
    }
    return query;
}
