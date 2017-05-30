#-------------------------------------------------
#
# Project created by QtCreator 2017-05-23T02:27:50
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StudentAbsenceTableApp
TEMPLATE = app


SOURCES += main.cpp\
    view/studentabsencedialogapp.cpp \
    view/HierarchicalHeaderView.cpp \
    model/studentabsencemodel.cpp

HEADERS  += \
    view/studentabsencedialogapp.h \
    view/HierarchicalHeaderView.h \
    model/studentabsencemodel.h
