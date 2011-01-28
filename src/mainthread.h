#ifndef MAINTHREAD_H
#define MAINTHREAD_H
#include <QObject>
#include <QtNetwork>
#include "logregthread.h"
#include "message.h"
#include "activeclients.h"
struct Type
{
    QString typeSend;
    QString typeLog;
    QString typeNotify;
    QString typeCommand;
};

struct parsValue
{
    QString prefix;
    QString separator;
    QString key;
    QString login;
    QString send;
    QString addfriend;
    QString removefriend;
};
class MainThread : public QObject
{
    Q_OBJECT
public:
    explicit MainThread(QCoreApplication* a);
    ~MainThread();
   // int PreParser(QString&);
private:
    Key keys;
    Type type;
    QSqlDatabase db;
    QCoreApplication* app;
    QTcpServer tcpServer;
    QSharedPointer <ActiveClients> cl;
    QSharedPointer <LogRegThread> LogReg_thread;
    QSharedPointer <ContactsBase> base;
    parsValue structure;
    bool addFriend(QTcpSocket*,  int, QString);
    bool removeFriend(QTcpSocket*, int, QString);
signals:
    void initialize();
    void authentification(QSharedPointer<IMessage> mes);
    void send(QSharedPointer<IMessage> mes);
public slots:
    void NewUser(int, UserItem);
    void AcceptConnection();
    void Read();
    void Disconnected();
    void Init();
    void Write(int,QString);
    void Send(QSharedPointer<IMessage>);
};

#endif // MAINTHREAD_H
