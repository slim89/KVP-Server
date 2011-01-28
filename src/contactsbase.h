#ifndef CONTACTSBASE_H
#define CONTACTSBASE_H
#include <QObject>
#include <QDebug>
#include <QtSql>
#include <QString>
#include "activeclients.h"
 #include <QStringList>
class ContactsBase : public QObject
{
public:
    ContactsBase(QSqlDatabase*);
    int WriteUser(QString , QString);
    bool connectDB();
    bool addFriend(int,QString);
    bool removeFriend(int,QString);
    UserItem isCorrectUser(int,QString);//возвращаем структуру данных
private:
    QSqlDatabase* db;
public slots:

};

#endif // CONTACTSBASE_H
