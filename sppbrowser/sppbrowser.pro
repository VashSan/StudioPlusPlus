#-------------------------------------------------
#
# Project created by QtCreator 2013-08-02T20:33:59
#
#-------------------------------------------------

QT       += widgets xml

TARGET = sppbrowser
TEMPLATE = lib

DEFINES += SPPBROWSER_LIBRARY

SOURCES += sppbrowser.cpp \
    solutionbrowser.cpp \
    solutionscanner.cpp

HEADERS += sppbrowser.h\
        sppbrowser_global.h \
    solutionbrowser.h \
    data.h \
    solutionscanner.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

FORMS += \
    solutionbrowser.ui
