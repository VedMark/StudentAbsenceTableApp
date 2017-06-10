#-------------------------------------------------
#
# Project created by QtCreator 2017-05-23T02:27:50
#
#-------------------------------------------------

QT       += core gui \
    xml \
    concurrent

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StudentAbsenceTableApp
TEMPLATE = app warn_on


SOURCES += main.cpp\
    model/studentabsencemodel.cpp \
    view/HierarchicalHeaderView.cpp \
    view/studentabsencedialogapp.cpp \
    view/studenttableview.cpp \
    controller/adddialog.cpp \
    controller/modelcontroller.cpp \
    controller/menucomponents.cpp \
    controller/removedialog.cpp \
    controller/xmlparser.cpp \
    view/proxymodel.cpp \
    controller/abstractfinddialog.cpp \
    controller/finddialog.cpp

HEADERS  += \
    model/studentabsencemodel.h \
    view/HierarchicalHeaderView.h \
    view/studentabsencedialogapp.h \
    view/studenttableview.h \
    controller/adddialog.h \
    controller/modelcontroller.h \
    controller/menucomponents.h \
    controller/removedialog.h \
    controller/xmlparser.h \
    view/proxymodel.h \
    controller/abstractfinddialog.h \
    controller/finddialog.h

RESOURCES += icons.qrc
