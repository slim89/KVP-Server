#ifndef UT_LOGREGTHREAD_H
#define UT_LOGREGTHREAD_H

#include <QObject>
#include <QDebug>
#include <QtTest/QTest>
#include "logregthread.h"
#include "contactsbase.h"


class Ut_LogRegThread : public QObject
{
    Q_OBJECT
    LogRegThread *thread;
    QSqlDatabase db;
    QSharedPointer <ContactsBase> base;

private slots:

    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();
};

#endif // UT_LOGREGTHREAD_H
