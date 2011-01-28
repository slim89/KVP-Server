#include "ut_logregthread.h"
#include <QSharedPointer>


void Ut_LogRegThread::initTestCase()
{
    Key key;
    db = QSqlDatabase::addDatabase("QSQLITE");
    base=QSharedPointer <ContactsBase>(new ContactsBase(&db));
    thread=new LogRegThread(key,base);
}

void Ut_LogRegThread::cleanupTestCase()
{
    delete thread;
}

void Ut_LogRegThread::init()
{    
    thread->start();
}

void Ut_LogRegThread::cleanup()
{    
    thread->terminate();
}

QTEST_APPLESS_MAIN(Ut_LogRegThread)
