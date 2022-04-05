QT += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts serialport

CONFIG += c++11

TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Cheque.cpp \
    DataBaseAccessor.cpp \
    Delivery.cpp \
    PieChart.cpp \
    Statistics.cpp \
    Trash.cpp \
    main.cpp \
    mainwindow.cpp \
    sideBar.cpp \
    chartview.cpp

HEADERS += \
    Cheque.h \
    CustomDialog.h \
    DataBaseAccessor.h \
    Delivery.h \
    PieChart.h \
    Statistics.h \
    Trash.h \
    mainwindow.h \
    sideBar.h \
    chartview.h

FORMS += \
    Cheque.ui \
    Delivery.ui \
    Statistics.ui \
    Trash.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    cash.png \
    checked-cash.png \
    checked-delivery.png \
    checked-statistics.png \
    checked-trash.png \
    delivery.png \
    statistics.png \
    trash.png \
    config.xml

