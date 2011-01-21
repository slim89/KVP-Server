#ifndef LOGREGTHREAD_H
#define LOGREGTHREAD_H

#include <QSharedPointer>
#include <QThread>
#include "message.h"
#include "activeclients.h"
#include "contactsbase.h"

struct Key
{
    QString s;
    QString r;
    QString m;
    QString l;
    QString p;
    QString o;
    QString id;
};

class LogRegThread : public QThread
{
    Q_OBJECT
private:
    QSharedPointer<ContactsBase> file;
    Key keys;
public:
    LogRegThread(Key ,QSharedPointer<ContactsBase>);
    void run();
    virtual ~ LogRegThread();
signals:
    void write(int,QString);
    void newuser(int,UserItem);
public slots:
    void ThreadMain(QSharedPointer<IMessage>);
};

#endif // LOGREGTHREAD_H
