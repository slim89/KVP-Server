include(../common_tests.pri)
TARGET = ut_message
SOURCES += \
ut_message.cpp \
$$SRCDIR/message.cpp
HEADERS += ut_message.h\
$$SRCDIR/message.h

QMAKE_CLEAN += $$TARGET \
Makefile
