#include "UserDao.h"
#include<QSqlQuery>
#include<QVariant>
UserDao::UserDao()
{
    //创建user表
    QSqlQuery query;
    query.exec("create table if not exists user(\
                   userid char(20),\
                   password char(20)\
               )");
}

bool UserDao::addUser(UserEntity &ue, bool &ok)
{
    QSqlQuery query;
    //判断userid在不在
    query.prepare("select * from user where userid=?");
    query.bindValue(0,ue.userid);
    bool execOk=query.exec();
    if(!execOk)
        return false;//sql语句执行失败

    if(query.next())//userid已经存在
    {
        ok=true;
        return true;
    }

    query.prepare("insert into user values(?,?)");
    query.bindValue(0,ue.userid);
    query.bindValue(1,ue.pwd);
    execOk=query.exec();
    if(!execOk)
        return false;//sql语句执行失败
    ok=false;//插入数据成功
    return true;
}

bool UserDao::checkUser(UserEntity &ue, bool &ok)
{
    QSqlQuery query;
    //判断userid在不在
    query.prepare("select * from user where userid = ? and password = ?");
    query.bindValue(0,ue.userid);
    query.bindValue(1,ue.pwd);
    bool execOk=query.exec();
    if(!execOk)
        return false;//sql语句执行失败
    if(query.next())
        ok=true;//能查到就是登录成功
    else
        ok=false;//查不到就是登录失败
    return true;
}
