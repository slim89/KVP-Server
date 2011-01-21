#include "ut_message.h"
void Ut_Message::initTestCase()
{

    pattern="#key1/val#'ue1#key2/value2#key3/value3";
    patternSmall="#key1/value1";
    key2="key2";
    value2="value2";
    key3="key3";
    value3="value3";
    value3New="value3New";
}

void Ut_Message::cleanupTestCase()
{

}

void Ut_Message::init()
{
    m=new Message(pattern);
}

void Ut_Message::cleanup()
{
    delete m;
}

void Ut_Message::Clone()
{
    m->Parse();
    newm=m->Clone();
    QCOMPARE(newm->Unparse(),pattern+"#/");
    delete newm;
}

void Ut_Message::CreateAndParse()
{
    Message localM(pattern);
    localM.Parse();
    QCOMPARE(localM.NumOfPair(),3);
    QCOMPARE(localM.Unparse(),pattern+"#/");

    m->Parse();
    QCOMPARE(((Message*)m)->NumOfPair(),3);
    QCOMPARE(m->Unparse(),pattern+"#/");
}

void Ut_Message::CreateAndParseStd()
{
    Message localM("");
    localM.Parse();
    QCOMPARE(localM.NumOfPair(),0);
    QString emptystr="";
    QCOMPARE(localM.Unparse(),emptystr+"#/");
}

void Ut_Message::AddAndReplace()
{
    Message localM(patternSmall);
    localM.Parse();
    QString value1="value1";
    QCOMPARE(localM.GetPart("key1"),value1);
    localM.AddPart(key2,value2);
    localM.AddPart(key3,value3);
    localM.ReplacePart(key3,value3New);
    QCOMPARE(localM.GetPart(key2),value2);
    QCOMPARE(localM.GetPart(key3),value3New);
    QString finalstr=patternSmall+"#"+key2+"/"+value2+"#"+key3+"/"+value3New+"#/";
    QCOMPARE(localM.Unparse(),finalstr);
    QString emptystr="";
    QCOMPARE(localM.GetPart("key5"),emptystr);
}
void Ut_Message::removePart()
{
    m->AddPart("key1","Value");
    QCOMPARE(m->Delete("key1"),true);

}

QTEST_APPLESS_MAIN(Ut_Message)
