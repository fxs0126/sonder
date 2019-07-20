
TEMPLATE= lib
TARGET = sc_log
CONFIG += release shared
INCLUDEPATH += ../include
HEADERS += ../include/sc_log.h 
QT -= core gui

SOURCES += sc_log.cpp
DESTDIR = ../../../lib

MOC_DIR = ../../../obj/common/sc_log/.moc 
UI_DIR	= ../../../obj/common/sc_log/.ui 
QMAKE_RPATHDIR = ../../../lib

LIBS += -L../../../lib -lsc_file \
        -L../../../lib -lsc_thread \
        -L../../../lib -lsc_sem \
        -L../../../lib -lsc_console \
        -L../../../lib -lsc_util
