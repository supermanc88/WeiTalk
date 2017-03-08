#ifndef WEITALKSERVERAPI_H
#define WEITALKSERVERAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QList>

//搜索到好友的信息
typedef struct user_info_s
{
    QString acNickName;
    QString acUserName;
    int nUserId;
}user_info_t;


//搜索到群组的信息
typedef struct group_info_s
{
    QString acGroupName;        //群组名称
    QString acGroupOwn;         //群组创建人
    QString acDescription;      //群组描述，简介
    int  nGroupCount;           //群组可容纳人数
    unsigned int nGroupId;      //群组ID
}group_info_t;

typedef struct group_member_s
{
    int authority;//权限
    QString username; //用户名
    QString remark;//未知的
    QString nickname;//昵称
    int userid;//用户id
    int groupid; //群组id
}group_member_t;

class WeiTalkServerAPI : public QObject
{
    Q_OBJECT
public:
    explicit WeiTalkServerAPI(QObject *parent = 0);

    static QByteArray getHtml(QString url);

    //查找好友
    static void SelectUserText(const QString username, QList<user_info_t> * userList);

    //查找群组
    static void SelectGroupText(const QString groupName, QList<group_info_t> * groupList);

    //获取已加入群组列表
    static void SelectGroupList(const QString username, QList<group_info_t> * groupList);

    /**
     * @brief SelectGroupUserList
     * @param groupId                   群组ID
     * @param groupMemberList           用来返回群组成员列表
     * @param memberCount               用来返回群组成员数量
     *
     * 获取群组中的成员
     */
    static void SelectGroupUserList(unsigned int groupId, QList<group_member_t> * groupMemberList, int& memberCount);



signals:

public slots:
};

#endif // WEITALKSERVERAPI_H
