#ifndef DATABASEACCESSOR_H
#define DATABASEACCESSOR_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

/*
 * Объект по работе с базой данных
 */
class DataBaseAccessor : public QObject
{
    Q_OBJECT

    QSqlDatabase* _db;

public:
    DataBaseAccessor();

    bool processRequest(QString uid, QVector<QString> &table_names, QVector<QVector<QString>> &results, QVector<QString> args);

protected:
    QString setArgs(QString query, QVector<QString> args);
};

#endif // DATABASEACCESSOR_H
