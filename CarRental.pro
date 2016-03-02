#-------------------------------------------------
#
# Project created by QtCreator 2016-01-12T07:04:31
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CarRental
TEMPLATE = app
CONFIG += c++11
RC_ICONS = images\icon.ico
CONFIG += debug

SOURCES += main.cpp\
        mainwindow.cpp \
    carblock.cpp \
    namedialog.cpp \
    returndialog.cpp \
    database.cpp \
    dbconfigdialog.cpp

HEADERS  += mainwindow.h \
    carblock.h \
    namedialog.h \
    returndialog.h \
    database.h \
    dbconfigdialog.h

FORMS    += mainwindow.ui \
    carblock.ui \
    namedialog.ui \
    returndialog.ui \
    dbconfigdialog.ui

RESOURCES += \
    resources.qrc
