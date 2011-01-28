#ifndef ACTIVECLIENTS_H
#define ACTIVECLIENTS_H
#include <QObject>
#include <QString>
#include <QDebug>
#include <QMap>
#include <QMapIterator>
#include <QSharedPointer>
#include <QtNetwork>
struct UserItem
{
    int ID;
    QString nick;
    QStringList friends_list;
};

class ActiveClients : public QObject
{
    Q_OBJECT
private:
    QMap<QTcpSocket*,UserItem> logedUserMap;
    QMap<QTcpSocket*,UserItem> tmpUserMap;
    unsigned int tmpnumber;
    int maxSize;
public:
    ActiveClients();
    bool Add(QTcpSocket*);
    bool Remove(QTcpSocket*);
    bool Remove(int);
    int Size();
    QTcpSocket* SetLoged(int, UserItem) ;
    QMapIterator<QTcpSocket*,UserItem> TmpUsers() const;
    QMapIterator<QTcpSocket*,UserItem> LogedUsers() const;
    QTcpSocket* Socket(int) const;
    QString Nick(int);
    QTcpSocket* NotTmpSocket(int) const;
    int ID(QTcpSocket*) const;
    int NotTmpID(QTcpSocket*) const;
    void AllClear();
    void TmpClear();
    bool addFriend(QTcpSocket*, QString);
    bool removeFriends(QTcpSocket*, QString);
    virtual ~ActiveClients();
};

#endif // ACTIVECLIENTS_H
