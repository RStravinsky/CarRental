#-------------------------------------------------
#
# Project created by QtCreator 2016-01-12T07:04:31
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SigmaCars
TEMPLATE = app
CONFIG += c++11
RC_ICON = icon.ico
CONFIG += debug

SOURCES += main.cpp\
        mainwindow.cpp \
    carblock.cpp \
    namedialog.cpp \
    returndialog.cpp

HEADERS  += mainwindow.h \
    carblock.h \
    namedialog.h \
    returndialog.h

FORMS    += mainwindow.ui \
    carblock.ui \
    namedialog.ui \
    returndialog.ui

RESOURCES += \
    resources.qrc
