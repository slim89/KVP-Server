#include "contactsbase.h"

ContactsBase::ContactsBase(QSqlDatabase* d)
{
    db=d;

}
int ContactsBase::WriteUser(QString nick, QString pass)
{
    qCritical()<<"000";
    QString insert ="INSERT INTO accounts(nick ,password, friends) VALUES  (:nick,:password ,:friends)";
    QSqlQuery query = QSqlQuery(insert,*db);
    query.bindValue(":nick", nick);
    query.bindValue(":password", pass);
    QString admin ="777";
    query.bindValue(":friends", admin);//изначально у пользователя нет друзей
    qCritical()<<nick<<"  "<<pass;
    bool res= query.exec();

    if (!res)
    {
         qCritical()<<"Insert error";
         return -1;
    }
    qCritical()<<"111";
    QString max ="SELECT MAX(id) FROM accounts";
    QSqlQuery query1 = QSqlQuery(max, *db );

    qCritical()<<"222";

    query1.exec();

    qCritical()<<"333";

    if (!query1.isActive() )
            qCritical()<<query1.lastError().text();

    qCritical()<<"444";

    while ( query1.next() )
    {
        int id = query1.value(0).toInt();
        if (id > 0)
        {
            qCritical()<<id;
            qCritical()<<"WRITE SUCCESS";
            return id;


        }
        else
            return -1;
    }
    qCritical()<<"6666";

    return -1;
}
bool ContactsBase::connectDB()
{
   db->setDatabaseName("./acc.sqlite");
   //db.setUserName(QString name);
   //db.setPassword(QString password);
   if (!db->open())
   {
        qCritical()<<"Base not open";
        return false;
   }
   QString table = "CREATE TABLE IF NOT EXISTS accounts(id INTEGER PRIMARY KEY, nick TEXT, password TEXT, friends TEXT);";
   QSqlQuery query =  QSqlQuery(table, *db );
   bool res = query.exec();
   if (!res)
   {
        qCritical()<<"Create table error";
        return false;
    }
   qCritical()<<"Base open successful";

   ///////FOR TEST///////////////////////////////
  WriteUser("Vasya", "12");
  WriteUser("Anton", "12");

   addFriend(1, "2");
   addFriend(1, "3");
   addFriend(2, "1");
   addFriend(2, "3");


  /* removeFriend(1,"1");
   removeFriend(1,"2");
   removeFriend(1,"3");
   removeFriend(1,"4");*/
/////////////////////////////////////


   return true;
}

UserItem ContactsBase::isCorrectUser(int id,QString pass)
{
    QString check ="SELECT nick, friends FROM accounts WHERE id= :id AND password = :password";
    QString nick;
    QString friends;
    QStringList friends_list;
    QSqlQuery query(check, *db);
    query.bindValue(":id", id);
    query.bindValue(":password", pass);
    query.exec();
    UserItem res;
   if ( ! query.isActive() )
              qCritical()<<query.lastError().text();

   while ( query.next() )
   {
        nick = query.value(0).toString();
        friends = query.value(1).toString();
        friends_list=friends.split("/");//распарсили запись

        if (nick !="" && friends !="")
        {

            res.nick = nick;
            res.ID =id;

            for(int i=0;i<friends_list.size(); i++)
            {
                res.friends_list.append(friends_list.at(i));
            }

            return res;
        }
    }
   return res;
}
bool ContactsBase::addFriend(int id, QString myfriend)
{
    QString get_friends ="SELECT friends FROM accounts WHERE id= :id";
    QSqlQuery query(get_friends, *db);
    query.bindValue(":id", id);
    query.exec();
    if ( ! query.isActive() )
                qCritical()<<query.lastError().text();

    while ( query.next() )
    {
          get_friends = query.value(0).toString();//получили исходный список друзей
          qCritical()<<"ADD SEARCH"<<get_friends;

          QString set_friends ="UPDATE accounts SET friends= :friends  WHERE id= :id";
          QSqlQuery query1(set_friends, *db);
          get_friends = get_friends + "/"+ myfriend;
          query1.bindValue(":friends", get_friends);
          query1.bindValue(":id", id);
          bool res = query1.exec();
          if ( ! query1.isActive() )
                      qCritical()<<query1.lastError().text();
          return res;
    }

    return false;
}
bool ContactsBase::removeFriend(int id, QString myfriend)
{
    QString get_friends ="SELECT friends FROM accounts WHERE id= :id";
    QSqlQuery query(get_friends, *db);
    query.bindValue(":id", id);
    query.exec();
    if ( ! query.isActive() )
                qCritical()<<query.lastError().text();
    while ( query.next() )
    {
          get_friends = query.value(0).toString();//получили исходный список друзей
          QString set_friends ="UPDATE accounts SET friends= :friends  WHERE id= :id";
          QString del_friend = "/" + myfriend;
          if(get_friends.contains(del_friend))//если есть такой друг
          {
                qCritical()<<"DO UDALENIYA"<<get_friends;

                get_friends.remove(del_friend);
                qCritical()<<"POSLE UDALENIYA"<<get_friends;
          }
          else
                return false;
          QSqlQuery query1(set_friends, *db);
          query1.bindValue(":friends", get_friends);
          query1.bindValue(":id", id);
          bool res = query1.exec();
          if ( ! query1.isActive() )
                      qCritical()<<query1.lastError().text();
          return res;
    }

    return false;

}
