# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = sc_testlog
DESTDIR = ../../../bin
QT -= core gui
CONFIG += console
CONFIG += release shared

INCLUDEPATH += ../include
SOURCES += sc_test.cpp
LIBS += -L../../../lib -lsc_log \
        -L../../../lib -lsc_file \
        -L../../../lib -lsc_sem \
        -L../../../lib -lsc_thread \
        -L../../../lib -lsc_util

MOC_DIR += ../../../obj/common/sc_test/.moc
UI_DIR += ../../../obj/common/sc_test/.ui
RCC_DIR += ../../../obj/common/sc_test/.rcc