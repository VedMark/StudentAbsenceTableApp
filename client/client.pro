#-------------------------------------------------
#
# Project created by QtCreator 2017-06-12T22:45:44
#
#-------------------------------------------------

QT       += core gui \
    network \
    xml

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
    studentabsenceclient.cpp \
    model/studentabsencemodel.cpp \
    view/HierarchicalHeaderView.cpp \
    view/proxymodel.cpp \
    view/studentabsencedialogapp.cpp \
    controller/abstractfinddialog.cpp \
    controller/adddialog.cpp \
    controller/finddialog.cpp \
    controller/menucomponents.cpp \
    controller/modelcontroller.cpp \
    controller/removedialog.cpp \
    controller/xmlparser.cpp

HEADERS += \
    studentabsenceclient.h \
    model/studentabsencemodel.h \
    view/HierarchicalHeaderView.h \
    view/proxymodel.h \
    view/studentabsencedialogapp.h \
    controller/abstractfinddialog.h \
    controller/adddialog.h \
    controller/finddialog.h \
    controller/menucomponents.h \
    controller/modelcontroller.h \
    controller/removedialog.h \
    controller/xmlparser.h

RESOURCES += icons.qrc
