include(../common_tests.pri)
TARGET = ut_logregthread
target.path = /usr/share/bin
INSTALLS += target
SOURCES += \
ut_logregthread.cpp \
$$SRCDIR/logregthread.cpp\
$$SRCDIR/contactsbase.cpp
HEADERS += ut_logregthread.h\
$$SRCDIR/logregthread.h\
$$SRCDIR/contactsbase.h
