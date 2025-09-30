#ifndef HISTORYDAO_H
#define HISTORYDAO_H
#include<QString>
#include<vector>
#include<QDateTime>
using namespace std;

struct HistoryEntity
{
    QString userid;//用户id,区分哪个用户
    QString name;//歌曲时间
    QDateTime dateTime;//听歌时间
};


class HistoryDao
{
public:
    HistoryDao();

    /*
     * 插入历史信息
    */
    bool addHistory(HistoryEntity& he);
    /*
     * 查看所有历史信息
     * 参数1:用户信息
     * 参数2:返回的数据
    */
    bool findHistory(HistoryEntity& he,vector<HistoryEntity>& vec);
    /*
     * 清空历史信息
     * 参数1:用户信息
    */
    bool clearHistory(HistoryEntity& he);
};

#endif // HISTORYDAO_H
