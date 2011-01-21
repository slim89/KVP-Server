QT += network sql
QT += core
QT -= gui
CONFIG += console thread
CONFIG -= app_bundle
SOURCES += main.cpp \
 message.cpp \
 activeclients.cpp \
 mainthread.cpp \
 logregthread.cpp \
    contactsbase.cpp
HEADERS += message.h \
 activeclients.h \
 mainthread.h \
 logregthread.h \
    contactsbase.h

unix { 

#VARIABLES
isEmpty(PREFIX) {
  PREFIX = /usr
}
BINDIR = $$PREFIX/bin
DATADIR =$$PREFIX/share
DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"
#MAKE INSTALL
TARGET = kvp-server
target.path = /usr/bin
INSTALLS += target
}
