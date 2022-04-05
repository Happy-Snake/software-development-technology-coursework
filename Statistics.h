#ifndef STATISTICS_H
#define STATISTICS_H

#include <QWidget>
#include <QStandardItemModel>
#include <QChart>
#include <QLineSeries>

#include "chartview.h"
#include "DataBaseAccessor.h"

namespace Ui {
class Statistics;
}

class Statistics : public QWidget
{
    Q_OBJECT

public:
    explicit Statistics(DataBaseAccessor* dbAccessor, QWidget *parent = nullptr);
    ~Statistics();

private slots:
    void on_productsCB_currentIndexChanged(const QString &arg1);

    void on_productsCB_3_currentIndexChanged(const QString &arg1);
    
    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::Statistics *ui;
    DataBaseAccessor *_dbAccessor;
    QStandardItemModel *_model;
    QStandardItemModel *_model_2;

    ChartView *graph;
    QChart *chart;
    QLineSeries *series_income;
    QLineSeries *series_expenses;
    QLineSeries *series_profit;

    ChartView *graph_2;
    QChart *chart_2;
    QLineSeries *series_visits;

    QVector<QVector<QString>> _productsList;
    QVector<QVector<QString>> _coffeeHouseList;

    QVector<QColor> _colors = {
        QColor(250, 235, 215),
        QColor( 0, 255, 255),
        QColor(127, 255, 212),
        QColor(240, 255, 255),
        QColor(245, 245, 220),
        QColor(255, 228, 196),
        QColor(255, 235, 205),
        QColor( 0, 0, 255),
        QColor(138, 43, 226),
        QColor(165, 42, 42),
        QColor(222, 184, 135),
        QColor( 95, 158, 160),
        QColor(127, 255, 0),
        QColor(210, 105, 30),
        QColor(255, 127, 80),
        QColor(100, 149, 237),
        QColor(255, 248, 220),
        QColor(220, 20, 60),
        QColor( 0, 255, 255),
        QColor( 0, 0, 139),
        QColor( 0, 139, 139),
        QColor(184, 134, 11),
        QColor(169, 169, 169),
        QColor( 0, 100, 0),
        QColor(169, 169, 169),
        QColor(189, 183, 107),
        QColor(139, 0, 139),
        QColor( 85, 107, 47),
        QColor(255, 140, 0),
        QColor(153, 50, 204),
        QColor(139, 0, 0),
        QColor(233, 150, 122),
        QColor(143, 188, 143),
        QColor( 72, 61, 139),
        QColor( 47, 79, 79),
        QColor( 47, 79, 79),
        QColor( 0, 206, 209),
        QColor(148, 0, 211),
        QColor(255, 20, 147),
        QColor( 0, 191, 255),
        QColor(105, 105, 105),
        QColor(105, 105, 105),
        QColor( 30, 144, 255),
        QColor(178, 34, 34),
        QColor(255, 250, 240),
        QColor( 34, 139, 34),
        QColor(255, 0, 255),
        QColor(220, 220, 220),
        QColor(248, 248, 255),
        QColor(255, 215, 0),
        QColor(218, 165, 32),
        QColor(128, 128, 128),
        QColor(128, 128, 128),
        QColor( 0, 128, 0),
        QColor(173, 255, 47),
        QColor(240, 255, 240),
        QColor(255, 105, 180),
        QColor(205, 92, 92),
        QColor( 75, 0, 130),
        QColor(255, 255, 240),
        QColor(240, 230, 140),
        QColor(230, 230, 250),
        QColor(255, 240, 245),
        QColor(124, 252, 0),
        QColor(255, 250, 205),
        QColor(173, 216, 230),
        QColor(240, 128, 128),
        QColor(224, 255, 255),
        QColor(250, 250, 210),
        QColor(211, 211, 211),
        QColor(144, 238, 144),
        QColor(211, 211, 211),
    };
};

#endif // STATISTICS_H
