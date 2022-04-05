#include "Statistics.h"
#include "ui_Statistics.h"

#include "PieChart.h"

#include <QTableView>
#include <QSplitter>
#include <QDebug>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QDateTime>
#include <float.h>
#include <cmath>

#define SELECT_PRODUCT_LIST             "391c3b26-4c64-11ec-b5d2-ff63cead543a"
#define SELECT_PRODUCT_STATISTICS       "9d828786-4b1b-11ec-b715-6375352888de"
#define SELECT_ALL_PRODUCTS_STATISTICS  "64abc8ce-4c78-11ec-b5d3-cb515e3a68d4"
#define SELECT_WRITTEN_OFF              "f7f011de-4d50-11ec-a93f-dbd7fddb705e"
#define SELECT_SOLD                     "13bf97a4-4d51-11ec-a940-6fc9f61f9871"
#define SELECT_ACTIVE                   "d24a0ed0-4d55-11ec-a941-472b3e4fb273"
#define SELECT_ALL_CATEGORIES           "bfa4d16e-4d57-11ec-a942-af98ff680700"
#define SELECT_MONEY_STATISTICS         "b16c227c-53ac-11ec-8c2c-57ab7fb252cd"
#define SELECT_CH_MONEY_STATISTICS      "7e4b42f2-55d1-11ec-af0f-1bb3358f169d"
#define SELECT_COFFEE_HOUSES            "f78e346e-4f9e-11ec-8b94-6f3d897a4d47"
#define SELECT_HOUR_STATISTICS          "e05b741e-55e3-11ec-a6a2-df785a037266"
#define SELECT_CH_HOUR_STATISTICS       "93590a30-55f4-11ec-a6a3-272a55215d13"

Statistics::Statistics(DataBaseAccessor* dbAccessor, QWidget *parent) :
    QWidget(parent)
    , ui(new Ui::Statistics)
    , _dbAccessor(dbAccessor)
    , _model(nullptr)
    , _model_2(nullptr)
    , chart(new QChart)
    , series_income(new QLineSeries)
    , series_expenses(new QLineSeries)
    , series_profit(new QLineSeries)
    , chart_2(new QChart)
    , series_visits(new QLineSeries)
{
    ui->setupUi(this);

    // ----------------- set product statistic ---------------------
    QVector<QString> table_names;
    QVector<QVector<QString>> results;
    QVector<QString> args;
    if (!_dbAccessor->processRequest(SELECT_PRODUCT_LIST, table_names, results, args)) {
        qWarning() << "Product list select failed";
        return;
    }
    _productsList = results;

    ui->productsCB->addItem("Всего");
    for (QVector<QString> line : results)
        ui->productsCB->addItem(line[1]);

    ui->tableView->setFocusPolicy(Qt::NoFocus);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->sortByColumn(0,Qt::AscendingOrder);
    ui->tableView->resizeColumnsToContents();
    // --------------------------------------------------------------

    // ----------------- set product statistic ---------------------
    ui->productsCB_3->addItem("Все категории");
    ui->productsCB_3->addItem("В активе");
    ui->productsCB_3->addItem("Продано");
    ui->productsCB_3->addItem("Списано");

    ui->tableView_3->setFocusPolicy(Qt::NoFocus);
    ui->tableView_3->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_3->sortByColumn(0,Qt::AscendingOrder);
    ui->tableView_3->resizeColumnsToContents();
    // --------------------------------------------------------------

    // ------------------ set income statistics ---------------------
    table_names.clear();
    results.clear();
    args.clear();
    if (!_dbAccessor->processRequest(SELECT_COFFEE_HOUSES, table_names, results, args)) {
        qWarning() << "Product list select failed";
        return;
    }
    _coffeeHouseList = results;

    series_income->setName("Доходы");
    chart->addSeries(series_income);
    series_expenses->setName("Расходы");
    chart->addSeries(series_expenses);
    series_profit->setName("Прибыль");
    chart->addSeries(series_profit);
    chart->setBackgroundVisible(false);

    QDateTimeAxis* xAxis = new QDateTimeAxis;
    xAxis->setRange(QDateTime::currentDateTime().addMonths(-5), QDateTime::currentDateTime());
    xAxis->setTickCount(6);
    xAxis->setFormat("MMMM");
    xAxis->setGridLineVisible(false);
    chart->addAxis(xAxis, Qt::AlignBottom);
    series_income->attachAxis(xAxis);
    series_expenses->attachAxis(xAxis);
    series_profit->attachAxis(xAxis);

    QValueAxis* yAxis = new QValueAxis;
    yAxis->setTitleFont(QFont("MS Shell Dlg 2", 10, QFont::Normal, true));
    yAxis->setTitleText("Сумма, р");
    yAxis->setGridLineVisible(false);
    chart->addAxis(yAxis, Qt::AlignLeft);
    series_income->attachAxis(yAxis);
    series_expenses->attachAxis(yAxis);
    series_profit->attachAxis(yAxis);

    graph = new ChartView(chart);
    graph->setRenderHint(QPainter::Antialiasing);

    ui->verticalLayout->addWidget(graph);

    ui->comboBox_2->addItem("Все кофейни");
    for (QVector<QString> row : results)
        ui->comboBox_2->addItem(row[1]);
    // --------------------------------------------------------------

    // ------------------ set visit statistics ----------------------

    chart_2->addSeries(series_visits);
    chart_2->setBackgroundVisible(false);

    QDateTimeAxis* xAxis_2 = new QDateTimeAxis;
    xAxis_2->setRange(QDateTime(QDate::currentDate(),QTime(0,0)),
                    QDateTime(QDate::currentDate(),QTime(23,59,59)));
    xAxis_2->setTickCount(21);
    xAxis_2->setFormat("hh");
    xAxis_2->setGridLineVisible(false);
    chart_2->addAxis(xAxis_2, Qt::AlignBottom);
    series_visits->attachAxis(xAxis_2);

    QValueAxis* yAxis_2 = new QValueAxis;
    yAxis_2->setTitleFont(QFont("MS Shell Dlg 2", 10, QFont::Normal, true));
    yAxis_2->setTitleText("Посещаемость, чел");
    yAxis_2->setGridLineVisible(false);
    chart_2->addAxis(yAxis_2, Qt::AlignLeft);
    series_visits->attachAxis(yAxis_2);

    graph_2 = new ChartView(chart_2);
    graph_2->setRenderHint(QPainter::Antialiasing);

    ui->verticalLayout_2->addWidget(graph_2);

    ui->comboBox->addItem("Все кофейни");
    for (QVector<QString> row : results)
        ui->comboBox->addItem(row[1]);
}

Statistics::~Statistics()
{
    delete ui;
}

void Statistics::on_productsCB_currentIndexChanged(const QString &arg1)
{
    QString id;
    for (QVector<QString> product : _productsList)
        if (product[1] == arg1)
            id = product[0];

    QVector<QString> table_names;
    QVector<QVector<QString>> results;
    QVector<QString> args;

    bool status = false;
    if (id.isEmpty()) {
        status = _dbAccessor->processRequest(SELECT_ALL_PRODUCTS_STATISTICS, table_names, results, args);
    } else {
        args.append(id);
        status = _dbAccessor->processRequest(SELECT_PRODUCT_STATISTICS, table_names, results, args);
    }
    if (!status) {
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
}

void Statistics::on_productsCB_3_currentIndexChanged(const QString &arg1)
{
    QVector<QString> table_names;
    QVector<QVector<QString>> results;
    QVector<QString> args;

    bool status = false;
    if (arg1 == "Списано") {
        status = _dbAccessor->processRequest(SELECT_WRITTEN_OFF, table_names, results, args);
    } else if (arg1 == "Продано") {
        status = _dbAccessor->processRequest(SELECT_SOLD, table_names, results, args);
    } else if (arg1 == "В активе") {
        status = _dbAccessor->processRequest(SELECT_ACTIVE, table_names, results, args);
    } else {
        status = _dbAccessor->processRequest(SELECT_ALL_CATEGORIES, table_names, results, args);
    }

    if (!status) {
        qWarning() << "Statistics select failed";
        return;
    }

    if (!_model_2) {
        _model_2 = new QStandardItemModel(0, 2);

        _model_2->setHeaderData(0, Qt::Horizontal, tr("Абонент"));
        _model_2->setHeaderData(1, Qt::Horizontal, tr("Номер телефона"));

        ui->tableView_3->setModel(_model_2);
        ui->pieChart_3->setModel(_model_2);

        QItemSelectionModel *selectionModel = new QItemSelectionModel(_model_2);
        ui->tableView_3->setSelectionModel(selectionModel);
        ui->pieChart_3->setSelectionModel(selectionModel);
        ui->tableView_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->tableView_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->pieChart_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->pieChart_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        QHeaderView *headerView = ui->tableView_3->horizontalHeader();
        headerView->setStretchLastSection(true);
    } else {
        _model_2->removeRows(0, _model_2->rowCount());
    }

    for (int row = 0; row < results.size(); row++) {
        _model_2->insertRow(_model_2->rowCount());
        _model_2->setData(_model_2->index(row, 0), results[row][0]);
        _model_2->setData(_model_2->index(row, 1), Qt::AlignCenter, Qt::TextAlignmentRole);
        _model_2->setData(_model_2->index(row, 1), results[row][1]);
        _model_2->setData(_model_2->index(row, 0), QColor(_colors[row]), Qt::DecorationRole);
    }

    ui->pieChart_3->setFocus();
}

void Statistics::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    QString id;
    for (QVector<QString> coffeeHouse : _coffeeHouseList)
        if (coffeeHouse[1] == arg1)
            id = coffeeHouse[0];

    QVector<QString> table_names;
    QVector<QVector<QString>> results;
    QVector<QString> args;

    bool status = false;
    if (id.isEmpty()) {
        status = _dbAccessor->processRequest(SELECT_MONEY_STATISTICS, table_names, results, args);
    } else {
        args.append(id);
        status = _dbAccessor->processRequest(SELECT_CH_MONEY_STATISTICS, table_names, results, args);
    }
    if (!status) {
        qWarning() << "Statistics select failed";
        return;
    }

    double max = DBL_MIN;
    double min = DBL_MAX;
    for (QVector<QString> row : results)
        for (QString valueStr : row) {
            double value = valueStr.toDouble();
            if (value < min)
                min = value;
            if (value > max)
                max = value;
        }
    min = (round(min * 100)/100 < 0) ? min * 1.1 : 0;
    max = (round(max * 100)/100 > 0) ? max * 1.1 : 0;

    series_income->clear();
    series_expenses->clear();
    series_profit->clear();
    chart->axisY()->setRange(min, max);

    for (QVector<QString> row : results) {
        QDateTime xValue;
        if (row[0].replace( " ", "" ) == "January") {
            int diff = 1 - QDate::currentDate().month();
            xValue = QDateTime::currentDateTime().addMonths(diff);
        } else if (row[0].replace( " ", "" ) == "February") {
            int diff = 2 - QDate::currentDate().month();
            xValue = QDateTime::currentDateTime().addMonths(diff);
        } else if (row[0].replace( " ", "" ) == "March") {
            int diff = 3 - QDate::currentDate().month();
            xValue = QDateTime::currentDateTime().addMonths(diff);
        } else if (row[0].replace( " ", "" ) == "April") {
            int diff = 4 - QDate::currentDate().month();
            xValue = QDateTime::currentDateTime().addMonths(diff);
        } else if (row[0].replace( " ", "" ) == "May") {
            int diff = 5 - QDate::currentDate().month();
            xValue = QDateTime::currentDateTime().addMonths(diff);
        } else if (row[0].replace( " ", "" ) == "June") {
            int diff = 6 - QDate::currentDate().month();
            xValue = QDateTime::currentDateTime().addMonths(diff);
        } else if (row[0].replace( " ", "" ) == "July") {
            int diff = 7 - QDate::currentDate().month();
            xValue = QDateTime::currentDateTime().addMonths(diff);
        } else if (row[0].replace( " ", "" ) == "August") {
            int diff = 8 - QDate::currentDate().month();
            xValue = QDateTime::currentDateTime().addMonths(diff);
        } else if (row[0].replace( " ", "" ) == "September") {
            int diff = 9 - QDate::currentDate().month();
            xValue = QDateTime::currentDateTime().addMonths(diff);
        } else if (row[0].replace( " ", "" ) == "October") {
            int diff = 10 - QDate::currentDate().month();
            xValue = QDateTime::currentDateTime().addMonths(diff);
        } else if (row[0].replace( " ", "" ) == "November") {
            int diff = 11 - QDate::currentDate().month();
            xValue = QDateTime::currentDateTime().addMonths(diff);
        } else if (row[0].replace( " ", "" ) == "December") {
            int diff = 12 - QDate::currentDate().month();
            xValue = QDateTime::currentDateTime().addMonths(diff);
        }

        series_profit->append(xValue.toMSecsSinceEpoch(), row[1].toDouble());
        series_income->append(xValue.toMSecsSinceEpoch(), row[2].toDouble());
        series_expenses->append(xValue.toMSecsSinceEpoch(), row[3].toDouble());
    }

}

void Statistics::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QString id;
    for (QVector<QString> coffeeHouse : _coffeeHouseList)
        if (coffeeHouse[1] == arg1)
            id = coffeeHouse[0];

    QVector<QString> table_names;
    QVector<QVector<QString>> results;
    QVector<QString> args;

    bool status = false;
    if (id.isEmpty()) {
        status = _dbAccessor->processRequest(SELECT_HOUR_STATISTICS, table_names, results, args);
    } else {
        args.append(id);
        status = _dbAccessor->processRequest(SELECT_CH_HOUR_STATISTICS, table_names, results, args);
    }
    if (!status) {
        qWarning() << "Statistics select failed";
        return;
    }

    double max = DBL_MIN;
    double min = DBL_MAX;
    for (QVector<QString> row : results){
        double value = row[1].toDouble();
        if (value < min)
            min = value;
        if (value > max)
            max = value;
    }
    min = (round(min * 100)/100 < 0) ? min * 1.1 : 0;
    max = (round(max * 100)/100 > 0) ? max * 1.1 : 0;

    series_visits->clear();
    chart_2->axisY()->setRange(min, max);

    for (QVector<QString> row : results) {
        int hour = row[0].toInt();
        double value = row[1].toDouble();
        series_visits->append(QDateTime(QDate::currentDate(), QTime(hour,0)).toMSecsSinceEpoch(),
                              value);
    }
}
