#ifndef UT_ACTIVECLIENTS_H
#define UT_ACTIVECLIENTS_H

#include <QObject>
#include "activeclients.h"
#include <QtTest/QTest>
#include <QDebug>
#include <QString>

class Ut_ActiveClients : public QObject
{
    Q_OBJECT

    ActiveClients* ActClients;
    QString empty;
    /*QString pattern;
    QString patternSmall;
    QString key2;
    QString value2;
    QString key3;
    QString value3;
    QString value3New;*/
private slots:

    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();
    //TESTS
    void addRemove();
    void addRemoveFriend();
    void setLoged();
};

#endif // UT_ACTIVECLIENTS_H
