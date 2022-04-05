#ifndef TRASH_H
#define TRASH_H

#include <QWidget>

#include "DataBaseAccessor.h"

namespace Ui {
class Trash;
}

class Trash : public QWidget
{
    Q_OBJECT

public:
    explicit Trash(DataBaseAccessor* dbAccessor, QWidget *parent = nullptr);
    ~Trash();

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_comboBox_2_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::Trash *ui;
    DataBaseAccessor *_dbAccessor;
    QString _curCoffeeHouseId;
    QString _curProductId;
    QVector<QVector<QString>> _coffeeHouses;
    QVector<QVector<QString>> _productsAmount;
};

#endif // TRASH_H
