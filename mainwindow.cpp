#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _statisticsWidget(nullptr)
    , _chequeWidget(nullptr)
    , _trash(nullptr)
    , _delivery(nullptr)
    , _dbAccessor(new DataBaseAccessor)
{
    ui->setupUi(this);
    connect(ui->sideBar, SIGNAL(actionChanged(QString)), this, SLOT(onActionChanged(QString)));
    ui->sideBar->addAction("Статистика", "statistics.png");
    ui->sideBar->addAction("Провести чек", "cash.png");
    ui->sideBar->addAction("Списать продукт", "trash.png");
    ui->sideBar->addAction("Поставки", "delivery.png");
    ui->title->setIndent(20);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    qApp->quit();
}

void MainWindow::onActionChanged(QString name)
{
    if (name == "statistics.png"){
        if (!_statisticsWidget)
            _statisticsWidget = new Statistics(_dbAccessor, this);
        ui->stackedWidget->addWidget(_statisticsWidget);
        ui->stackedWidget->setCurrentWidget(_statisticsWidget);
        ui->title->setText("Статистика");
    }
    if (name == "cash.png"){
        if (!_chequeWidget)
            _chequeWidget = new Cheque(_dbAccessor, this);
        ui->stackedWidget->addWidget(_chequeWidget);
        ui->stackedWidget->setCurrentWidget(_chequeWidget);
        ui->title->setText("Добавление чека");
    }
    if (name == "trash.png"){
        if (!_trash)
            _trash = new Trash(_dbAccessor, this);
        ui->stackedWidget->addWidget(_trash);
        ui->stackedWidget->setCurrentWidget(_trash);
        ui->title->setText("Списание продукта");
    }
    if (name == "delivery.png"){
        if (!_delivery)
            _delivery = new Delivery(_dbAccessor, this);
        ui->stackedWidget->addWidget(_delivery);
        ui->stackedWidget->setCurrentWidget(_delivery);
        ui->title->setText("Поставки");
    }

}
