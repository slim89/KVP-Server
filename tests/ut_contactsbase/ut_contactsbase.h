#ifndef UT_CONTACTSBASE_H
#define UT_CONTACTSBASE_H
#include <QObject>
#include <QDebug>
#include <QtTest/QTest>
#include <QtSql>
#include <QSharedPointer>
#include "contactsbase.h"
class Ut_ContactsBase : public QObject
{
    Q_OBJECT
    QSqlDatabase db;
    ContactsBase* base;
    //QSharedPointer <ContactsBase> base;
private slots:
    void initTestCase();
    void cleanupTestCase();
    void WriteUser();
    void friends();
    // Executed once before every test case
    void init();
        // Executed once after every test case
    void connectBase();
    void cleanup();


};

#endif // UT_CONTACTSBASE_H
