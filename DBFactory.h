#ifndef DBFACTORY_H
#define DBFACTORY_H
#include<QSqlDatabase>
#include"HistoryDao.h"
#include"UserDao.h"

//数据库工厂模式,使用简单工厂模式,封装成单例,用于工厂的类一般都是单例的
//工厂模式保证数据库不会重复打开(单例模式)
//保证数据库操作之前数据库一定是打开的。
class DBFactory
{
public:
    static DBFactory* getInstance();
    static void releaseInstance();
    inline UserDao* getUserDao(){return userDao;}
    inline HistoryDao* getHistoryDao(){return historyDao;}
private:
    DBFactory();
    DBFactory(const DBFactory& other){Q_UNUSED(other)}
    ~DBFactory();
    static DBFactory* instance;
    QSqlDatabase db;
    //把dao对象创建在堆空间,避免重复
    UserDao* userDao;
    HistoryDao* historyDao;
};

#endif // DBFACTORY_H
