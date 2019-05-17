######################################################################
# Automatically generated by qmake (3.0) Sun Apr 14 11:40:58 2019
######################################################################

TEMPLATE = app
TARGET = qmemo
INCLUDEPATH += .

QT += qml quick sql widgets core

CONFIG += debug_and_release
           
# Input
HEADERS += src/datahandler.hpp \
           src/fileinfomodel.hpp \
           src/fileinfoproxy.hpp

SOURCES += src/main.cpp \
           src/datahandler.cpp \
           src/fileinfomodel.cpp \
           src/fileinfoproxy.cpp

RESOURCES += qml.qrc \
             i18n.qrc
           
TRANSLATIONS = i18n/qmemo_ja.ts \
               i18n/qmemo_hu.ts

lupdate_only{
SOURCES = resources/main.qml \
          resources/ListPaneForm.ui.qml \
          resources/EditPaneForm.ui.qml
}

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

Release:DESTDIR = release
Release:OBJECTS_DIR = release/.obj
Release:MOC_DIR = release/.moc
Release:RCC_DIR = release/.rcc
Release:UI_DIR = release/.ui

Debug:DESTDIR = debug
Debug:OBJECTS_DIR = debug/.obj
Debug:MOC_DIR = debug/.moc
Debug:RCC_DIR = debug/.rcc
Debug:UI_DIR = debug/.ui
