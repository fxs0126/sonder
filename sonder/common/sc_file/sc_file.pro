# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = lib
TARGET = sc_file
DESTDIR = ../../../lib
QT -= core gui

CONFIG += release shared
DEFINES += UNICODE
INCLUDEPATH += ../include
HEADERS += ../include/sc_file.h
SOURCES += sc_file.cpp

MOC_DIR += ../../../obj/common/sc_file/.moc 
UI_DIR += ../../../obj/common/sc_file/.ui
RCC_DIR += ../../../obj/common/sc_file/.rcc 


LIBS += -L../../../lib -lsc_thread \
        -L../../../lib -lsc_sem \
        -L../../../lib -lsc_util
