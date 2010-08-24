#-------------------------------------------------
#
# Project created by QtCreator 2010-08-24T15:35:00
#
#-------------------------------------------------

QT       += core gui

TARGET = Agenda
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    AgendaTopic.cpp

HEADERS  += MainWindow.h \
    AgendaTopic.h

FORMS    += MainWindow.ui

CONFIG += mobility
MOBILITY = 

symbian {
    TARGET.UID3 = 0xe7192d21
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}
