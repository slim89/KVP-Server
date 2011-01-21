#include <QtCore/QCoreApplication>
#include <QDebug>
#include "mainthread.h"
#include <QObject>
#include <QEventLoop>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MainThread MThread(&a);

    return a.exec();
}
