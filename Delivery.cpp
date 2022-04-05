#include "Delivery.h"
#include "ui_Delivery.h"

#include <QDebug>
#include <QMessageBox>

#include "DataBaseAccessor.h"

#define SELECT_COFFEE_HOUSES        "f78e346e-4f9e-11ec-8b94-6f3d897a4d47"
#define SELECT_PRODUCT_LIST         "391c3b26-4c64-11ec-b5d2-ff63cead543a"
#define SELECT_FREQUENCY            "6c2b9b90-5058-11ec-beef-8387f0a59c63"
#define UPDATE_SCHEDULE             "4bd2f24a-5061-11ec-bef0-5b124b8588cc"
#define SELECT_PRODUCT_STATISTIC    "50a93dac-508d-11ec-a6dd-2bf08fb685be"

Delivery::Delivery(DataBaseAccessor *dbAccessor, QWidget *parent) :
    QWidget(parent)
    , ui(new Ui::Delivery)
    , _dbAccessor(dbAccessor)
    , _model (nullptr)
{
    ui->setupUi(this);
    ui->listWidget_2->hide();
    ui->frame->hide();
    ui->pushButton->hide();
    ui->widget->hide();

    // Запрашиваем кофейни
    QVector<QString> table_names;
    QVector<QVector<QString>> results;
    QVector<QString> args;
    if (!_dbAccessor->processRequest(SELECT_COFFEE_HOUSES, table_names, results, args)) {
        qWarning() << "Coffee houses list select failed";
        return;
    }
    _coffeeHouses = results;

    for (QVector<QString> coffeeHouse : _coffeeHouses) {
        QListWidgetItem* item = new QListWidgetItem(QString("\n   ") + coffeeHouse[1] + QString("\t\n"));
        ui->listWidget->addItem(item);
    }
    ui->listWidget->setFixedWidth(ui->listWidget->sizeHintForColumn(0) + ui->listWidget->frameWidth() * 2);

    ui->tableView->setFocusPolicy(Qt::NoFocus);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->sortByColumn(0,Qt::AscendingOrder);
    ui->tableView->resizeColumnsToContents();

}

Delivery::~Delivery()
{
    delete ui;
}

void Delivery::on_listWidget_currentTextChanged(const QString &currentText)
{
    for (QVector<QString> coffeeHouse : _coffeeHouses)
        if (currentText.contains(coffeeHouse[1]))
            _currentCoffeeHouseId = coffeeHouse[0];

    QVector<QString> table_names;
    QVector<QVector<QString>> results;
    QVector<QString> args;
    if (!_dbAccessor->processRequest(SELECT_PRODUCT_LIST, table_names, results, args)) {
        qWarning() << "Coffee houses list select failed";
        return;
    }
    _products = results;

    ui->listWidget_2->clear();
    for (QVector<QString> coffeeHouse : _products) {
        QListWidgetItem* item = new QListWidgetItem(QString("\n   ") + coffeeHouse[1] + QString("\t\n"));
        ui->listWidget_2->addItem(item);
    }
    ui->listWidget_2->setFixedWidth(ui->listWidget_2->sizeHintForColumn(0) + ui->listWidget_2->frameWidth() * 2);
    ui->listWidget_2->show();
}

void Delivery::on_listWidget_2_currentTextChanged(const QString &currentText)
{
    for (QVector<QString> product : _products)
        if (currentText.contains((product[1])))
            _currentProduct = product[0];

    QVector<QString> table_names;
    QVector<QVector<QString>> results;
    QVector<QString> args;
    args.append(_currentCoffeeHouseId);
    args.append(_currentProduct);
    if (!_dbAccessor->processRequest(SELECT_FREQUENCY, table_names, results, args)) {
        qWarning() << "Coffee houses list select failed";
        return;
    }

    _currentFrequensy = results[0][0].toInt();
    ui->spinBox->setValue(_currentFrequensy);
    _currentNumber = results[0][1].toInt();
    ui->spinBox_2->setValue(_currentNumber);

    // Рисуем руговую диаграмму
    table_names.clear();
    results.clear();
    args.clear();
    args.append(_currentProduct);
    args.append(_currentCoffeeHouseId);

    if (!_dbAccessor->processRequest(SELECT_PRODUCT_STATISTIC, table_names, results, args)) {
        qWarning() << "Statistics select failed";
        return;
    }

    if (!_model) {
        _model = new QStandardItemModel(0,2);

        ui->tableView->setModel(_model);
        ui->pieChart->setModel(_model);

        QItemSelectionModel *selectionModel = new QItemSelectionModel(_model);
        ui->tableView->setSelectionModel(selectionModel);
        ui->pieChart->setSelectionModel(selectionModel);
        ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->pieChart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->pieChart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        QHeaderView *headerView = ui->tableView->horizontalHeader();
        headerView->setStretchLastSection(true);
    } else {
        _model->removeRows(0, _model->rowCount());
    }

    for (int row = 0; row < table_names.size(); row++) {
        _model->insertRow(_model->rowCount());
        if (table_names[row] == "active")
            _model->setData(_model->index(row, 0), "В обороте");
        else if (table_names[row] == "sold")
            _model->setData(_model->index(row, 0), "Продано");
        else if (table_names[row] == "written_off")
            _model->setData(_model->index(row, 0), "Списано");

        _model->setData(_model->index(row, 1), Qt::AlignCenter, Qt::TextAlignmentRole);
        _model->setData(_model->index(row, 1), results[0][row]);
        _model->setData(_model->index(row, 0), QColor(_colors[row]), Qt::DecorationRole);
    }

    ui->pieChart->setFocus();

    ui->frame->show();
    ui->pushButton->show();
    ui->widget->show();
    ui->pushButton->setDisabled(true);
}

void Delivery::on_spinBox_valueChanged(int arg1)
{
    if (_currentFrequensy != ui->spinBox->value()
            || _currentNumber != ui->spinBox_2->value())
        ui->pushButton->setDisabled(false);
    else
        ui->pushButton->setDisabled(true);
}

void Delivery::on_spinBox_2_valueChanged(int arg1)
{
    if (_currentFrequensy != ui->spinBox->value()
            || _currentNumber != ui->spinBox_2->value())
        ui->pushButton->setDisabled(false);
    else
        ui->pushButton->setDisabled(true);
}

void Delivery::on_pushButton_clicked()
{
    QVector<QString> table_names;
    QVector<QVector<QString>> results;
    QVector<QString> args;
    args.append(QString::number(ui->spinBox->value()));
    args.append(QString::number(ui->spinBox_2->value()));
    args.append(_currentProduct);
    args.append(_currentCoffeeHouseId);
    if (!_dbAccessor->processRequest(UPDATE_SCHEDULE, table_names, results, args)) {
        QMessageBox msgBox;
        msgBox.setText("Произошла ошибка при изменении графика поставок.");
        msgBox.exec();
        return;
    }

    _currentFrequensy = ui->spinBox->value();
    _currentNumber = ui->spinBox_2->value();
    ui->pushButton->setDisabled(true);

    QMessageBox msgBox;
    msgBox.setText("График поставок успешно изменен.");
    msgBox.exec();
}
