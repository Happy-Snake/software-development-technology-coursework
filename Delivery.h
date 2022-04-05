#ifndef DELIVERY_H
#define DELIVERY_H

#include <QWidget>
#include <QStandardItemModel>

#include "DataBaseAccessor.h"

namespace Ui {
class Delivery;
}

class Delivery : public QWidget
{
    Q_OBJECT

public:
    explicit Delivery(DataBaseAccessor *dbAccessor, QWidget *parent = nullptr);
    ~Delivery();

private slots:
    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_listWidget_2_currentTextChanged(const QString &currentText);

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_pushButton_clicked();

private:
    Ui::Delivery *ui;
    QVector<QVector<QString>> _coffeeHouses;
    QString _currentCoffeeHouseId;
    QVector<QVector<QString>> _products;
    QString _currentProduct;
    int _currentFrequensy;
    int _currentNumber;
    QStandardItemModel *_model;

    DataBaseAccessor* _dbAccessor;

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

#endif // DELIVERY_H
