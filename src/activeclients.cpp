#include "activeclients.h"

//CONSTRUCTOR--------------------------------------------------------------------
ActiveClients::ActiveClients()
{
    tmpnumber=0;
}

//SIZE---------------------------------------------------------------------------
int ActiveClients::Size()
{
    return logedUserMap.size()+tmpUserMap.size();
}

//ADD----------------------------------------------------------------------------
bool ActiveClients::Add(QTcpSocket* sock)
{
        //qCritical()<<"ADD "<<sock;
        tmpnumber++;
        tmpUserMap[sock].ID=-tmpnumber;
        return true;
}

//REMOVE-------------------------------------------------------------------------
bool ActiveClients::Remove(QTcpSocket* sock)
{
    int result=logedUserMap.remove(sock);
    if (result==0)
    {
        result=tmpUserMap.remove(sock);
        //qCritical()<<"REMOVE TMP";
    }
    else
    {
        //qCritical()<<"REMOVE NOT TMP";
    }
    return result;
}

//REMOVE-------------------------------------------------------------------------
bool  ActiveClients::Remove(int ID)
{
    return this->Remove(this->Socket(ID));
}

//SETLOGED-----------------------------------------------------------------------
QTcpSocket* ActiveClients::SetLoged(int tmpID, UserItem user)
{
    //qCritical()<<"SETLOGED";
    QMapIterator<QTcpSocket*,UserItem> iter(tmpUserMap);
    while (iter.hasNext())
    {
        iter.next();
        if (iter.value().ID==tmpID)
        {
            tmpUserMap.remove(iter.key());
            logedUserMap[iter.key()].ID=user.ID;
            logedUserMap[iter.key()].nick=user.nick;
            logedUserMap[iter.key()].friends_list=user.friends_list;//записали  в мапу данные из базы
            //qCritical()<<logedUserMap[iter.key()];
            //qCritical()<<"SETLOGED END";
            return iter.key();
        }
    }
    //qCritical()<<"SETLOGED END";
    return NULL;
}


QString ActiveClients::Nick(int ID)
{
    QString name;
    QMapIterator<QTcpSocket*,UserItem> iter(logedUserMap);
    while (iter.hasNext())
    {
        iter.next();
        if (iter.value().ID==ID)
            name=iter.value().nick;
    }
    return name;
}

//LOGEDUSERS--------------------------------------------------------------------
QMapIterator<QTcpSocket*,UserItem> ActiveClients::LogedUsers() const
{
    QMapIterator<QTcpSocket*,UserItem> iter(logedUserMap);
    return iter;
}

//TMPUSERS----------------------------------------------------------------------
QMapIterator<QTcpSocket*,UserItem> ActiveClients::TmpUsers() const
{
    QMapIterator<QTcpSocket*,UserItem> iter(tmpUserMap);
    return iter;
}

//SOCKET------------------------------------------------------------------------
QTcpSocket* ActiveClients::Socket(int ID) const
{
    QMapIterator<QTcpSocket*,UserItem> iter(logedUserMap);
    while (iter.hasNext())
    {
        iter.next();
        if (iter.value().ID==ID)
            return iter.key();
    }
    QMapIterator<QTcpSocket*,UserItem> iterTmp(tmpUserMap);
    while (iterTmp.hasNext())
    {
        iterTmp.next();
        if (iterTmp.value().ID==ID)
            return iterTmp.key();
    }
    return NULL;
}

//NOTTMPSOCKET------------------------------------------------------------------
QTcpSocket* ActiveClients::NotTmpSocket(int ID) const
{
    qCritical()<<ID;
    QMapIterator<QTcpSocket*,UserItem> iter(logedUserMap);
    while (iter.hasNext())
    {
        iter.next();
        qCritical()<<iter.value().ID<<"  "<<iter.value().nick;
        if (iter.value().ID==ID)
        {
            qCritical()<<iter.key();
            return iter.key();
        }
    }
    return NULL;
}

//NICK--------------------------------------------------------------------------
int ActiveClients::ID(QTcpSocket* sock) const
{
    int tmp=0;
    tmp=logedUserMap[sock].ID;
    if (tmp==0)
        tmp=tmpUserMap[sock].ID;
    return tmp;
}

//NOTTMPNICK--------------------------------------------------------------------
int ActiveClients::NotTmpID(QTcpSocket* sock) const
{
    int tmp=0;
    tmp=logedUserMap[sock].ID;
    return tmp;
}

//ALLCLEAR----------------------------------------------------------------------
void ActiveClients::AllClear()
{
    logedUserMap.clear();
    tmpUserMap.clear();
    tmpnumber=0;
}

//TMPCLEAR----------------------------------------------------------------------
void ActiveClients::TmpClear()
{
    tmpUserMap.clear();
    tmpnumber=0;
}

//DESTRUCTOR--------------------------------------------------------------------
ActiveClients::~ActiveClients()
{
    QMapIterator<QTcpSocket*,UserItem> iter(logedUserMap);
    while (iter.hasNext())
    {
        iter.next();
        iter.key()->disconnectFromHost();
        iter.key()->waitForDisconnected();
        iter.key()->deleteLater();
    }
    QMapIterator<QTcpSocket*,UserItem> iterTmp(tmpUserMap);
    while (iterTmp.hasNext())
    {
        iterTmp.next();
        iterTmp.key()->disconnectFromHost();
        iterTmp.key()->deleteLater();
    }
    logedUserMap.clear();
    tmpUserMap.clear();
}
bool ActiveClients::addFriend(QTcpSocket* sock, QString friends)
{

   QMapIterator<QTcpSocket*,UserItem> iter(tmpUserMap);
    while (iter.hasNext())
    {

        iter.next();
        if (iter.key()== sock)
        {
            qCritical()<<"ACTIV CLIENTS FRIEND LIST"<<logedUserMap[iter.key()].friends_list;
            if(!logedUserMap[iter.key()].friends_list.contains(friends ))
            {
                logedUserMap[iter.key()].friends_list.append(friends);
                qCritical()<<"ACTIV CLIENTS FRIEND LIST AFTER ADD FRIEND"<<logedUserMap[iter.key()].friends_list;
                return true;
            }
        }
    }
    return false;
}
bool ActiveClients::removeFriends(QTcpSocket* sock, QString friends){
    QMapIterator<QTcpSocket*,UserItem> iter(tmpUserMap);
     while (iter.hasNext())
     {

         iter.next();
         if (iter.key()== sock)
         {
             qCritical()<<"ACTIV CLIENTS FRIEND LIST"<<logedUserMap[iter.key()].friends_list;
             if(logedUserMap[iter.key()].friends_list.contains(friends ))
             {
                 for (int i = 0; i < logedUserMap[iter.key()].friends_list.size(); ++i)
                 {
                    if(logedUserMap[iter.key()].friends_list.at(i)==friends)
                    {
                        logedUserMap[iter.key()].friends_list.removeAt(i);
                        qCritical()<<"ACTIV CLIENTS FRIEND LIST AFTER REMOVE FRIEND"<<logedUserMap[iter.key()].friends_list;
                        return true;
                   }
                 }
                 //logedUserMap[iter.key()].friends_list.remove( friends +"/");

             }
             else
                return false;
         }
     }
     return false;
}
