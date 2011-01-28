#include "mainthread.h"
#include <QDebug>
#include <QTcpServer>
#include <QFile>
#include "contactsbase.h"

//INIT--------------------------------------------------------------------------
void MainThread::Init()
{

    tcpServer.setMaxPendingConnections(100);
    if (!tcpServer.listen(QHostAddress::Any, 3425))
    {
        qCritical()<<"Server Listen Error";
        app->exit(1);
        return;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    base=QSharedPointer <ContactsBase>(new ContactsBase(&db));
    base->connectDB();


    cl = QSharedPointer <ActiveClients>(new ActiveClients());

    //study our preParser
    keys.s="s";
    keys.r="r";
    keys.m="m";
    keys.l="l";
    keys.p="p";
    keys.o="o";
    keys.id="id";
    type.typeLog="log";
    type.typeSend="send";


    structure.separator ="/";
    structure.prefix ="#";
    structure.key= "type";
    structure.addfriend="addfr";
    structure.removefriend="rfr";
    structure.login= "log";
    structure.send="send";


    LogReg_thread = QSharedPointer <LogRegThread>(new LogRegThread(keys, base));
    LogReg_thread->start();
    connect(&tcpServer, SIGNAL(newConnection()),this, SLOT(AcceptConnection()),Qt::QueuedConnection);
    connect(this,SIGNAL(authentification(QSharedPointer<IMessage>)),LogReg_thread.data(),SLOT(ThreadMain(QSharedPointer<IMessage>)));
    connect(this,SIGNAL(send(QSharedPointer<IMessage>)),this,SLOT(Send(QSharedPointer<IMessage>)));
    connect(LogReg_thread.data(),SIGNAL(write(int,QString)),this,SLOT(Write(int,QString)));
    connect(LogReg_thread.data(),SIGNAL(newuser(int,UserItem)),this,SLOT(NewUser(int,UserItem)));
}

//NEWUSER--------------------------------------------------------------------------
void MainThread::NewUser(int tmpID,  UserItem user)
{
    qCritical()<<"NEWUSER";
    QTcpSocket* sock;
    if (cl->Socket(user.ID)==NULL)
    {
        sock = cl->SetLoged(tmpID, user);////????????????????возвращает сокет зачем нужна след функция?!

        qCritical()<<"NEWUSER SetLoged - return sock"<<sock ;
        if (sock!=NULL)
        {
            qCritical()<<"Sock!=0"<<sock;
            sock->write("#s/ser*ver#m/good login-password#/");
        }
    }
    else
    {
        if ((sock=cl->Socket(tmpID))!=NULL)
        {
            sock->write("#s/ser*ver#m/login allready exist#/");
            return;
        }
    }
    //////////////////////////////////////////
    IMessage* mes=new Message("");//сообщение что подключился такой то клиент
    mes->AddPart(keys.o,"connect");
    QString str_ID=QString::number(user.ID);
    mes->AddPart(keys.id,str_ID);
    mes->AddPart(keys.s,cl->Nick(user.ID));
    QString buf=mes->Unparse();//message all ysers about connect client
    QMapIterator<QTcpSocket*,UserItem> iter=cl->LogedUsers();
    while (iter.hasNext())//всем шлем  такой то//
    {
        iter.next();

        if (user.friends_list.contains(QString::number(iter.value().ID)))
        { 

            iter.key()->write(qPrintable(buf));//ALL SEND
            qCritical()<<"BUFFER"<<buf;
        }

    }
   iter.toBack();
    while (iter.hasPrevious())
    {
        iter.previous();
        if (user.friends_list.contains(QString::number(iter.value().ID)))
        {
             mes->ReplacePart(keys.s,iter.value().nick);
             mes->ReplacePart(keys.id,QString::number(iter.value().ID));
             buf=mes->Unparse();//message client about connect each user from activlist
             sock->write(qPrintable(buf));//ALL SEND
        }
    }

    delete mes;
    return;
}

//CONSTRUCTOR---------------------------------------------------------------------
MainThread::MainThread(QCoreApplication* a) : QObject()
{
    app=a;
    connect(this, SIGNAL(initialize()),this, SLOT(Init()),Qt::QueuedConnection);
    emit initialize();
}

//ACCEPTCONNECTION----------------------------------------------------------------
void MainThread::AcceptConnection()
{
    QTcpSocket* sock= tcpServer.nextPendingConnection();
    if (!cl->Add(sock))
    {
        sock->write("#s/ser*ver#m/denial of service#/");
        qCritical()<<"denial";
        sock->close();
        sock->deleteLater();
        return;
    }
    connect(sock, SIGNAL(readyRead()),this, SLOT(Read()),Qt::QueuedConnection);
    connect(sock, SIGNAL(disconnected()),this, SLOT(Disconnected()),Qt::QueuedConnection);
    qCritical()<<"acepted ";
}

//DESTRUCTOR---------------------------------------------------------------------
MainThread::~MainThread()
{
    qCritical()<<"MainThread Destructor";
    if (!LogReg_thread.isNull())
    {
        if (LogReg_thread->isRunning())
        {
            LogReg_thread->exit();
            LogReg_thread->wait();
        }
        LogReg_thread.clear();
    }  
    cl.clear();
    tcpServer.close();
}

//SEND---------------------------------------------------------------------------
void MainThread::Send(QSharedPointer<IMessage> mes)
{
    QString sender,receiver,message;
    mes->Parse();
    sender=mes->GetPart(keys.s);

    qCritical()<<"sender"<<sender;

    if (sender.isEmpty()||sender.contains("-"))
    {
        return;
    }
    receiver=mes->GetPart(keys.id);
    qCritical()<<"receiver"<<receiver;
    if (receiver.isEmpty())
    {

        return;
    }
   ///-------------------- message=mes->GetPart(keys.m);//Modified Andrey 17/01/11
    //mes->Delete(keys.id);///////////////////////////////////////-------------------------------------------------
    mes->Delete(keys.s);//IF error then send nick in "#s/..."
    //mes->AddPart("id",sender);
    mes->ReplacePart(keys.id,sender);
    message=mes->Unparse();
    mes.clear();
    qCritical()<<"sender"<<sender;
    qCritical()<<"parse message is: "<<message;
    QTcpSocket* sock=cl->NotTmpSocket(receiver.toInt());//now socket
    if (sock!=NULL)
        sock->write(qPrintable(message));
}

//WRITE--------------------------------------------------------------------------
void MainThread::Write(int ID,QString buf)
{
   QTcpSocket* sock=cl->Socket(ID);
   if (sock!=NULL)
       sock->write(qPrintable(buf));
}

//DISCONNECTED-------------------------------------------------------------------
void MainThread::Disconnected()
{
    QTcpSocket *sock= ((QTcpSocket*)(this->sender()));
    int ID=cl->NotTmpID(sock);
    cl->Remove(sock);
    sock->close();
    sock->deleteLater();
    if (ID>0)
    {
        IMessage* mes=new Message("");
        mes->AddPart(keys.o,"disconnect");
        QString str_ID=QString::number(ID);
        mes->AddPart(keys.id,str_ID);
        QString buf=mes->Unparse();
        delete mes;
        QMapIterator<QTcpSocket*,UserItem> iter=cl->LogedUsers();
        while (iter.hasNext())//всем шлем кроме себя что отключился такой то
        {
            iter.next();
            iter.key()->write(qPrintable(buf));//ALL SEND ;//сокет отключившегося уже удален
        }
    }
    qCritical()<<"disconected ";
}
//READ----------------------------------------------------------------------------------
void MainThread::Read()
{
    qCritical()<<"READ ";
    QTcpSocket* sock= ((QTcpSocket*)(this->sender()));
    QByteArray array=sock->readAll();
    QString buffer = array;
    int id;
    id=cl->ID(sock);
    qCritical()<<buffer;
    if (!buffer.contains("#/"))
    {
        sock->disconnectFromHost();
        return;
    }
    QStringList strList=buffer.split("#/");
    if (strList.isEmpty())
        return;
    QStringListIterator iter(strList);
    while(iter.hasNext())
    {
        QString buf=iter.next();
        if( buf.contains(structure.prefix+  structure.key+  structure.separator+structure.login+  structure.prefix) )
        {
            qCritical()<<"RES2";
            QSharedPointer<IMessage> mes(new Message(buf));
            mes->AddPart(keys.s,QString::number(cl->ID(sock)));
            emit authentification(mes);
            return;
        }
        if( buf.contains(structure.prefix+structure.key+structure.separator+structure.send+structure.prefix))
        {
            QSharedPointer<IMessage> mes(new Message(buf));
            mes->AddPart(keys.s,QString::number(cl->ID(sock)));
            emit send(mes);
            return;
        }
        if( buf.contains(structure.prefix+structure.key+structure.separator+structure.addfriend+structure.prefix))
        {
            addFriend(sock,  id,  buf);
            return;
        }
        if( buf.contains(structure.prefix+structure.key+structure.separator+structure.removefriend+structure.prefix))
        {
            removeFriend(sock,  id,  buf);
            return;
        }
    }
}
bool MainThread::addFriend(QTcpSocket* sock, int id, QString buf)
{
                QSharedPointer<IMessage> mes(new Message(buf));
                mes->Parse();
                QString friends=mes->GetPart("addfr");
                qCritical()<<"ADD FRIEND friend= "<<friends;
                mes->deleteLater();
                bool res =base->addFriend(id,friends);
                bool res2 = cl->addFriend(sock,friends);
                if(res && res2)
                {
                    qCritical()<<"ADD FRIEND SUCCESSFUL";
                    return true;
                }
                else
                {
                     qCritical()<<"ADD FRIEND ERROR";
                     return false;
                 }
}
bool MainThread::removeFriend(QTcpSocket* sock, int id, QString buf)
{
               QSharedPointer<IMessage> mes1(new Message(buf));
               mes1->Parse();
               QString friends=mes1->GetPart("rfr");
               qCritical()<<"REMOVE FRIEND friend= "<<friends;
               mes1->deleteLater();
               bool res =base->removeFriend(id,friends);
               bool res2 =cl->removeFriends(sock,friends);
               if(res && res2)
               {
                   qCritical()<<"REMOVE FRIEND SUCCESSFUL";
                   return true;
               }
               else
               {
                    qCritical()<<"REMOVE FRIEND ERROR";
                    return false;
                }
}
