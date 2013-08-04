#-------------------------------------------------
#
# Project created by QtCreator 2013-08-02T20:30:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Studio
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../sppbrowser/release/ -lsppbrowser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../sppbrowser/debug/ -lsppbrowser
else:unix: LIBS += -L$$OUT_PWD/../sppbrowser/ -lsppbrowser

INCLUDEPATH += $$PWD/../sppbrowser
DEPENDPATH += $$PWD/../sppbrowser
