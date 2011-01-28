include(../common_tests.pri)
TARGET = ut_activeclients
target.path = /usr/share/bin
INSTALLS += target
SOURCES += \
ut_activeclients.cpp \
$$SRCDIR/activeclients.cpp
HEADERS += ut_activeclients.h\
$$SRCDIR/activeclients.h

