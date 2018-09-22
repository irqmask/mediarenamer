#-------------------------------------------------
#
# Project created by QtCreator 2018-08-10T22:01:49
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MediaRenamer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fileextensionfilter.cpp \
    filedata.cpp \
    filelistmodel.cpp \
    filelist.cpp

HEADERS  += mainwindow.h \
    fileextensionfilter.h \
    filedata.h \
    filelistmodel.h \
    filelist.h

FORMS    += mainwindow.ui
