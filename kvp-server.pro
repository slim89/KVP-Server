# -------------------------------------------------
# Project created by QtCreator 2010-12-09T13:16:32
# -------------------------------------------------

QMAKEVERSION = $$[QMAKE_VERSION]
ISQT4 = $$find(QMAKEVERSION, ^[2-9])
isEmpty( ISQT4 ) {
    error("Use the qmake include with Qt4.4 or greater, on Debian that is qmake-qt4");
}
QMAKE_CLEAN += configure-stamp build-stampv

TEMPLATE = subdirs
SUBDIRS  = src tests
