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
    AgendaTopic.cpp \
    AgendaDialog.cpp

HEADERS  += MainWindow.h \
    AgendaTopic.h \
    AgendaDialog.h

FORMS    += MainWindow.ui \
    AgendaDialog.ui

CONFIG += mobility
MOBILITY = 

symbian {
    TARGET.UID3 = 0xe7192d21
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

maemo5 {
    QT       += dbus
}

#
# Targets for debian source and binary package creation
#
debian-src.commands = dpkg-buildpackage -S -r -us -uc -d
debian-bin.commands = dpkg-buildpackage -b -r -uc -d
debian-all.depends = debian-src debian-bin

#
# Clean all but Makefile
#
compiler_clean.commands = -$(DEL_FILE) $(TARGET)

QMAKE_EXTRA_TARGETS += debian-all debian-src debian-bin install compiler_clean

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += mce

    #VARIABLES
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }

    BINDIR = $$PREFIX/bin
    DATADIR =$$PREFIX/share

    DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

    #MAKE INSTALL

    INSTALLS += target desktop service iconxpm icon26 icon48 sounds

    target.path =$$BINDIR

    desktop.path = $$DATADIR/applications/hildon
    desktop.files += $${TARGET}.desktop

    service.path = $$DATADIR/dbus-1/services
    service.files += $${TARGET}.service

    iconxpm.path = $$DATADIR/pixmap
    iconxpm.files += ../data/maemo/$${TARGET}.xpm

    icon26.path = $$DATADIR/icons/hicolor/26x26/apps
    icon26.files += ../data/26x26/$${TARGET}.png

    icon48.path = $$DATADIR/icons/hicolor/48x48/apps
    icon48.files += ../data/48x48/$${TARGET}.png

    sounds.path = $$DATADIR/agenda/sounds
    sounds.files += $$DATADIR/agenda/sounds/KDE_Critical_Error.wav
    sounds.files += $$DATADIR/agenda/sounds/KDE_Notify.wav

#   icon64.path = $$DATADIR/icons/hicolor/64x64/apps
#   icon64.files += ../data/64x64/$${TARGET}.png
}
