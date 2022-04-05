#include "Cheque.h"
#include "ui_Cheque.h"

#include "CustomDialog.h"

#include <QtDebug>
#include <QDateTime>
#include <QMessageBox>

#define SELECT_COFFEE_HOUSES    "f78e346e-4f9e-11ec-8b94-6f3d897a4d47"
#define INSERT_CHEQUE           "bfee0962-4f9e-11ec-8b93-431ae2b5e74b"
#define INSERT_CHEQUE_PRODUCT   "4cd762ce-4fb3-11ec-8b95-9f25a058b283"
#define SELECT_PRODUCT_LIST     "391c3b26-4c64-11ec-b5d2-ff63cead543a"
#define SELECT_CURRENT_PRODUCTS "21cfbd72-4fba-11ec-8b96-73ad99f54dcc"
#define UPDATE_CURRENT_NUMBER   "eeb1d9f0-4fc5-11ec-8b97-0f61830d4aee"

Cheque::Cheque(DataBaseAccessor *dbAccessor, QWidget *parent) :
    QWidget(parent)
    , ui(new Ui::Cheque)
    , _dbAccessor(dbAccessor)
{
    ui->setupUi(this);

    QVector<QString> table_names;
    QVector<QVector<QString>> results;
    QVector<QString> args;
    if (!_dbAccessor->processRequest(SELECT_PRODUCT_LIST, table_names, results, args)) {
        qWarning() << "Coffee houses list select failed";
        return;
    }
    _products = results;

    table_names.clear();
    results.clear();
    args.clear();
    if (!_dbAccessor->processRequest(SELECT_COFFEE_HOUSES, table_names, results, args)) {
        qWarning() << "Coffee houses list select failed";
        return;
    }
    _coffee_houses = results;
    for (QVector<QString> line : results)
        ui->comboBox->addItem(line[1]);

    ui->dateEdit->setDate(QDateTime::currentDateTime().date());
    ui->timeEdit->setTime(QDateTime::currentDateTime().time());

    QStringList HorLabels, VerLabels;
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

Cheque::~Cheque()
{
    delete ui;
}

void Cheque::on_pushButton_clicked()
{
    // Проверяем наличие необходимых товаров
    QVector<QString> table_names;
    QVector<QVector<QString>> results;
    QVector<QString> args;

    for (QVector<QString> coffee_house : _coffee_houses) {
        if (coffee_house[1] == ui->comboBox->currentText()){
            args.append(coffee_house[0]);
            break;
        }
    }

    if (!_dbAccessor->processRequest(SELECT_CURRENT_PRODUCTS, table_names, results, args)) {
        QMessageBox msgBox;
        msgBox.setText("Произошла ошибка при добавлении чека.");
        msgBox.exec();
        return;
    }

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        if (!ui->tableWidget->item(i, 1) && !ui->tableWidget->item(i, 0))
            continue;

        QRegExp re("\\d*");
        if (!re.exactMatch(ui->tableWidget->item(i, 1)->text())) {
            QMessageBox msgBox;
            msgBox.setText("В столбце 'Количество' должны быть только цифры!");
            msgBox.exec();
            return;
        }

        if ((ui->tableWidget->item(i, 0) && !ui->tableWidget->item(i, 1)) ||
                ((!ui->tableWidget->item(i, 0) && ui->tableWidget->item(i, 1)))) {
            QMessageBox msgBox;
            msgBox.setText("Проверьте правильность заполнения позиций чека!");
            msgBox.exec();
            return;
        }

        bool isOk = false;
        for (QVector<QString> result : results) {
            if (result[2] == ui->tableWidget->item(i, 0)->text()) {
                if (result[1].toInt() < ui->tableWidget->item(i, 1)->text().toInt()) {
                    QMessageBox msgBox;
                    msgBox.setText(QString("В данном заведении недостаточно товара (%1)!").arg(result[2]));
                    msgBox.exec();
                    return;
                } else {
                    isOk = true;
                    break;
                }
            }
        }

        if (!isOk) {
            QMessageBox msgBox;
            msgBox.setText(QString("В данном заведении недостаточно товара (%1)!").arg(ui->tableWidget->item(i, 0)->text()));
            msgBox.exec();
            return;
        }
    }

    // Вставлям новый чек
    table_names.clear();
    results.clear();
    args.clear();

    QString dateTime = ui->dateEdit->date().toString("yyyy-MM-dd");
    dateTime += " " + ui->timeEdit->time().toString("hh:mm:ss");
    args.append(dateTime);

    QString coffee_house_id;
    for (QVector<QString> coffee_house : _coffee_houses) {
        if (coffee_house[1] == ui->comboBox->currentText()){
            coffee_house_id = coffee_house[0];
            args.append(coffee_house_id);
            break;
        }
    }

    if (!_dbAccessor->processRequest(INSERT_CHEQUE, table_names, results, args)) {
        QMessageBox msgBox;
        msgBox.setText("Произошла ошибка при добавлении чека.");
        msgBox.exec();
        return;
    }

    QString id = results[0][0];
    table_names.clear();
    results.clear();

    // Добавляем в базу выбранные продукты, вычитам их из текущего состояния
    int size = ui->tableWidget->rowCount();
    for (int i = 0; i < size; i++) {
        if (!ui->tableWidget->item(i, 0))
            continue;

        args.clear();
        args.append(id);

        QString product_id;
        for (QVector<QString> product : _products) {
            if (product[1] == ui->tableWidget->item(i, 0)->text()) {
                product_id = product[0];
                args.append(product_id);
            }
        }
        args.append(ui->tableWidget->item(i, 1)->text());
        if (!_dbAccessor->processRequest(INSERT_CHEQUE_PRODUCT, table_names, results, args)) {
            QMessageBox msgBox;
            msgBox.setText("Произошла ошибка при добавлении продукции.");
            msgBox.exec();
            return;
        }

        table_names.clear();
        results.clear();
        args.clear();
        args.append(ui->tableWidget->item(i, 1)->text());
        args.append(coffee_house_id);
        args.append(product_id);
        if (!_dbAccessor->processRequest(UPDATE_CURRENT_NUMBER, table_names, results, args)) {
            QMessageBox msgBox;
            msgBox.setText("Произошла ошибка при добавлении продукции.");
            msgBox.exec();
            return;
        }
    }

    QMessageBox msgBox;
    msgBox.setText("Чек успешно добавлен!");
    msgBox.exec();
    return;
}

void Cheque::on_tableWidget_cellClicked(int row, int column)
{
    if (column != 0)
        return;

    ui->frame->setFocus();

    QStringList itemList;
    for (QVector<QString> product :_products)
        itemList.append(product[1]);

    CustomDialog dialog(itemList);
    if (dialog.exec() == QDialog::Accepted)
    {
        if (!dialog.comboBox()->currentText().isEmpty()){
            QTableWidgetItem *item = new QTableWidgetItem(dialog.comboBox()->currentText());
            ui->tableWidget->setItem(row, column, item);
            ui->tableWidget->resizeColumnsToContents();
        } else {
            ui->tableWidget->removeRow(row);
        }
    }
}
