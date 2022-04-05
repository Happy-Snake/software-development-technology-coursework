#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Statistics.h"
#include "Cheque.h"
#include "Trash.h"
#include "Delivery.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void onActionChanged(QString name);

private:
    Ui::MainWindow *ui;

    Statistics          *_statisticsWidget;
    Cheque              *_chequeWidget;
    Trash               *_trash;
    Delivery            *_delivery;
    DataBaseAccessor    *_dbAccessor;
};
#endif // MAINWINDOW_H
