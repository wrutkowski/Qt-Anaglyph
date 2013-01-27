#-------------------------------------------------
#
# Project created by QtCreator 2013-01-26T15:15:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt-Anaglyph
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    anaglyph.cpp \
    qvectora.cpp \
    qmatrixa.cpp

HEADERS  += mainwindow.h \
    anaglyph.h \
    qvectora.h \
    qmatrixa.h \
    aline.h \
    point.h

FORMS    += mainwindow.ui
