

TEMPLATE = lib
TARGET = sc_time
QT -= core gui

CONFIG += debug static
DESTDIR = ../../../lib
HEADERS += ../include/sc_time.h
SOURCES += sc_time.cpp
CONFIG += debug static
MOC_DIR = ../../../obj/common/sc_time/.moc
UI_DIR  = ../../../obj/common/sc_time/.ui
RCC_DIR = ../../../obj/common/sc_time/.rcc
