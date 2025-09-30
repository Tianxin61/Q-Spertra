#ifndef USERDAO_H
#define USERDAO_H
#include<QString>

struct UserEntity
{
    QString userid;
    QString pwd;
};

class UserDao
{
public:
    UserDao();
    /*
     * 插入用户,
    */
    bool addUser(UserEntity& ue,bool& ok);//插入一个用户
    /*查询一个用户,用于登录
     * 参数1:用户的信息
     * 参数2:查询是否成功
    */
    bool checkUser(UserEntity& ue,bool& ok);
};

#endif // USERDAO_H
