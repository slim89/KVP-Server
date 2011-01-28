#include "ut_activeclients.h"
#include <QString>
#include <QMapIterator>

void Ut_ActiveClients::initTestCase()
{
    ActClients=new ActiveClients();
}

void Ut_ActiveClients::cleanupTestCase()
{
    delete ActClients;
}

void Ut_ActiveClients::init()
{

}

void Ut_ActiveClients::cleanup()
{
    ActClients->TmpClear();
    ActClients->AllClear();
}

void Ut_ActiveClients::addRemove()
{
    QTcpSocket sock[10];
    for (int i=0;i<10;i++)
    {
        ActClients->Add(&sock[i]);

    }
    
    QMapIterator<QTcpSocket*,UserItem> iter=ActClients->TmpUsers();

    int i=0;
    while (iter.hasNext())
    {
       iter.next();
       QCOMPARE(iter.key(),&sock[i]);
       QCOMPARE(ActClients->Remove(iter.key()),true);
       i++;

    }
}

void Ut_ActiveClients::addRemoveFriend()
{
    QTcpSocket sock[10];
    for (int i=0;i<10;i++)
    {
        ActClients->Add(&sock[i]);
        QCOMPARE(ActClients->addFriend(&sock[i],"1"),true);
        QCOMPARE(ActClients->addFriend(&sock[i],"2"),true);
        QCOMPARE(ActClients->removeFriends(&sock[i],"1"),true);
        QCOMPARE(ActClients->removeFriends(&sock[i],"2"),true);
        QCOMPARE(ActClients->removeFriends(&sock[i],"3"),false);

    }
}
void Ut_ActiveClients::setLoged()
{
    QTcpSocket sock[10];
    UserItem user[10];
    for (int i=0;i<10;i++)
    {
        user[i].friends_list.append("1");
        user[i].friends_list.append("2");
        user[i].friends_list.append("3");
        user[i].friends_list.append("4");
        user[i].ID=i;
        user[i].nick="User"+ QString::number(i);
        ActClients->Add(&sock[i]);
        int id = ActClients->ID(&sock[i]);
        QCOMPARE(ActClients->SetLoged(id,user[i]),&sock[i]);
        QCOMPARE(ActClients->NotTmpSocket(i),ActClients->Socket(i));
    }

}

QTEST_APPLESS_MAIN(Ut_ActiveClients)
