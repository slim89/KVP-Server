#ifndef UT_MESSAGE_H
#define UT_MESSAGE_H

#include <QObject>
#include <QDebug>
#include <QtTest/QTest>
#include "message.h"

class Ut_Message : public QObject
{
    Q_OBJECT

    QString pattern;
    QString patternSmall;
    QString key2;
    QString value2;
    QString key3;
    QString value3;
    QString value3New;
    IMessage* m;
    IMessage* newm;
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
    void CreateAndParse();
    void CreateAndParseStd();
    void AddAndReplace();
    void removePart();
    void Clone();

};

#endif // UT_MESSAGE_H
