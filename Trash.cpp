#include "Trash.h"
#include "ui_Trash.h"

#include <QDebug>
#include <QMessageBox>

#define SELECT_COFFEE_HOUSES            "f78e346e-4f9e-11ec-8b94-6f3d897a4d47"
#define SELECT_CURRENT_PRODUCTS_AMOUNT  "2b971a72-4fe1-11ec-9a08-5b66b49d7618"
#define UPDATE_CURRENT_NUMBER           "eeb1d9f0-4fc5-11ec-8b97-0f61830d4aee"
#define INSERT_PRODUCT_DISCARD          "7a374470-4fed-11ec-9a09-ffa684868af2"

Trash::Trash(DataBaseAccessor *dbAccessor, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Trash)
    , _dbAccessor(dbAccessor)
{
    ui->setupUi(this);

    QVector<QString> table_names;
    QVector<QVector<QString>> results;
    QVector<QString> args;
    if (!_dbAccessor->processRequest(SELECT_COFFEE_HOUSES, table_names, results, args)) {
        qWarning() << "Coffee houses list select failed";
        return;
    }
    _coffeeHouses = results;

    ui->comboBox->addItem(QString());
    for (QVector<QString> coffeeHouse : _coffeeHouses) {
        ui->comboBox->addItem(coffeeHouse[1]);
    }

    ui->frame_2->hide();
    ui->frame_3->hide();
    ui->pushButton->hide();
}

Trash::~Trash()
{
    delete ui;
}

void Trash::on_comboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == QString()) {
        ui->frame_2->hide();
        ui->frame_3->hide();
        ui->pushButton->hide();
        return;
    }

    QVector<QString> table_names;
    QVector<QVector<QString>> results;
    QVector<QString> args;

    for (QVector<QString> coffee_house : _coffeeHouses) {
        if (coffee_house[1] == ui->comboBox->currentText()){
            _curCoffeeHouseId = coffee_house[0];
            args.append(_curCoffeeHouseId);
            break;
        }
    }

    if (!_dbAccessor->processRequest(SELECT_CURRENT_PRODUCTS_AMOUNT, table_names, results, args)) {
        qWarning() << "Products list select failed";
        return;
    }

    _productsAmount = results;

    ui->comboBox_2->clear();
    ui->comboBox_2->addItem(QString());
    for (QVector<QString> product : _productsAmount) {
        ui->comboBox_2->addItem(product[1]);
    }
    ui->frame_2->show();
}

void Trash::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    if (arg1 == QString()) {
        ui->frame_3->hide();
        ui->pushButton->hide();
        return;
    }

    int amount = 0;
    for (QVector<QString> product : _productsAmount) {
        if (product[1] == ui->comboBox_2->currentText()){
            _curProductId = product[0];
            amount = product[2].toInt();
            break;
        }
    }

    ui->spinBox->setMaximum(amount);
    ui->frame_3->show();
    ui->label_4->setText(QString("Для списания доступно %1 шт.").arg(amount));
    ui->pushButton->show();
}

void Trash::on_pushButton_clicked()
{
    QVector<QString> table_names;
    QVector<QVector<QString>> results;
    QVector<QString> args;

    args.append(ui->spinBox->text());
    args.append(_curCoffeeHouseId);
    args.append(_curProductId);
    if (!_dbAccessor->processRequest(UPDATE_CURRENT_NUMBER, table_names, results, args)) {
        QMessageBox msgBox;
        msgBox.setText("Произошла ошибка при списании продукции.");
        msgBox.exec();
        return;
    }

    table_names.clear();
    results.clear();
    args.clear();
    args.append(_curProductId);
    args.append(_curCoffeeHouseId);
    args.append(ui->spinBox->text());
    if (!_dbAccessor->processRequest(INSERT_PRODUCT_DISCARD, table_names, results, args)) {
        QMessageBox msgBox;
        msgBox.setText("Произошла ошибка при списании продукции.");
        msgBox.exec();
        return;
    }

    QMessageBox msgBox;
    msgBox.setText("Товар успешно списан!");
    msgBox.exec();
    ui->spinBox->setMaximum(ui->spinBox->maximum() - ui->spinBox->value());
    ui->label_4->setText(QString("Для списания доступно %1 шт.").arg(ui->spinBox->maximum()));
    for (int i = 0; i < _productsAmount.size(); i++) {
        if (_productsAmount[i][0] == _curProductId) {
            _productsAmount[i][2] = QString::number(ui->spinBox->maximum());
            break;
        }
    }
    return;
}
