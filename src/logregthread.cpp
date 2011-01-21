#include "logregthread.h"

//CONSTRUCTOR-------------------------------------------------------------------
LogRegThread::LogRegThread(Key k, QSharedPointer<ContactsBase> contactsFile)
{
    keys=k;
    file = contactsFile;
}

//RUN---------------------------------------------------------------------------
void  LogRegThread::run()
{
    exec();
}

//THREADMAIN--------------------------------------------------------------------
void LogRegThread::ThreadMain(QSharedPointer<IMessage> mes)
{
    qCritical()<<"LoginThread start: ";
    QString login,pass,operation;
    int ID,IDnew;
    if (!mes.isNull())
    {
        qCritical()<<"Login";
        mes->Parse();
        qCritical()<<"End of parse";

        ///qCritical()<<"IDnew"<< IDnew;
        pass=mes->GetPart(keys.p);
        login=mes->GetPart(keys.l);
        qCritical()<<"pass"<< pass;
        ID=mes->GetPart(keys.s).toInt();
        qCritical()<<"ID"<< ID;
        operation=mes->GetPart(keys.o);//Rasparsili

        if(operation=="login")
        {
                IDnew=mes->GetPart(keys.id).toInt();
                qCritical()<<"IDnew  "<<IDnew;
                qCritical()<<ID;
                UserItem res=file->isCorrectUser(IDnew,pass);

                if(res.nick.isEmpty())//если поле в структуре имя пользователя пустое
                {
                       qCritical()<<"DONT WRITE CLIENT";
                       emit write(ID,"#s/ser*ver#m/error of login#/");
                       mes.clear();
                       return ;
                }
                else
                {
                       qCritical()<<"WRITE CLIENT";
                       emit newuser(ID,res);//отслылаем структурку + темпшный ник
                       mes.clear();
                       return ;
                }
        }
        if(operation=="registration")
        {
            qCritical()<<"REGISTRATION";
            qCritical()<<"REGISTRATION"<<pass;
            int id=file->WriteUser(login,pass);
            qCritical()<<"REGISTRATION"<<id;
            if(id>0)
            {

                qCritical()<<"IF";
                emit write(ID,"#s/ser*ver#m/good registration#id/"+QString::number(id)+"#/");
                mes.clear();
                return;
            }
        }
    }//if mes
    mes.clear();
}

//DESTRUCTOR--------------------------------------------------------------------
LogRegThread::~LogRegThread()
{
    file.clear();
}
