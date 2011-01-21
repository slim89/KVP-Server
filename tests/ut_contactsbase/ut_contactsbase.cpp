#include "ut_contactsbase.h"
void Ut_ContactsBase::initTestCase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    base = new ContactsBase(&db);
}

void Ut_ContactsBase::cleanupTestCase()
{
    base->deleteLater();
}

void Ut_ContactsBase::init()
{
    base->connectDB();
}

void Ut_ContactsBase::cleanup()
{
}
void Ut_ContactsBase::connectBase()
{
     QCOMPARE(base->connectDB(),true);
}
void Ut_ContactsBase::friends()
{

    QString nick1="nick1";
    QString pass1="pass1";
    int id1 = base->WriteUser(nick1,pass1);
    QCOMPARE(base->addFriend(id1,"1"),true);
    QCOMPARE(base->addFriend(id1,"2"),true);
    QCOMPARE(base->addFriend(id1,"3"),true);
    QCOMPARE(base->addFriend(800,"3"),false);

    QCOMPARE(base->removeFriend(id1,"1"),true);
    QCOMPARE(base->removeFriend(id1,"2"),true);
    QCOMPARE(base->removeFriend(id1,"3"),true);
    QCOMPARE(base->removeFriend(id1,"4"),false);
}
void Ut_ContactsBase::WriteUser()
{
    QString nick="nick";
    QString nick1="nick1";
    QString nick2="nick2";
    QString nick3="nick3";
    QString nick4="nick4";
    QString pass1="pass1";
    QString pass2="pass2";
    QString pass3="pass3";
    QString pass4="pass4";

    int id1 = base->WriteUser(nick1,pass1);
    int id2 = base->WriteUser(nick2,pass2);
    int id3 = base->WriteUser(nick3,pass3);
    int id4 = base->WriteUser(nick4,pass4);

    QCOMPARE(base->isCorrectUser(id1,pass1).nick,nick1);
    QCOMPARE(base->isCorrectUser(id2,pass2).nick,nick2);
    QCOMPARE(base->isCorrectUser(id3,pass3).nick,nick3);
    QCOMPARE(base->isCorrectUser(id4,pass4).nick,nick4);

}
QTEST_APPLESS_MAIN(Ut_ContactsBase)
