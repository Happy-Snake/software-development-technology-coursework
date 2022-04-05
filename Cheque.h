#ifndef CHEQUE_H
#define CHEQUE_H

#include <QWidget>
#include <QTableWidgetItem>

#include "DataBaseAccessor.h"

namespace Ui {
class Cheque;
}

class Cheque : public QWidget
{
    Q_OBJECT

public:
    explicit Cheque(DataBaseAccessor* dbAccessor, QWidget *parent = nullptr);
    ~Cheque();

private slots:
    void on_pushButton_clicked();

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::Cheque *ui;
    DataBaseAccessor *_dbAccessor;

    QVector<QVector<QString>> _coffee_houses;
    QVector<QVector<QString>> _products;
};

#endif // CHEQUE_H
