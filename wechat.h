#ifndef WECHAT_H
#define WECHAT_H

#include <QWidget>
#include "weitalkserverapi.h"

class QVBoxLayout;
class GroupMemberModel;

namespace Ui {
class WeChat;
}

class WeChat : public QWidget
{
    Q_OBJECT

public:
    explicit WeChat(int groupId, QWidget *parent = 0);
    ~WeChat();

    /**
     * @brief initGroupMemberList
     *
     * 初始化群组成员列表，将成员放到滚动窗口中
     */
    void initGroupMemberList();

    QString getGroupName() const;
    void setGroupName(const QString &value);


    QMap<QString, GroupMemberModel *> *getGroupMemberMap() const;

private:
    Ui::WeChat *ui;
    int groupId;
    QString groupName;

    int memberCount;

    WeiTalkServerAPI API;

    QList<group_member_t> * groupMemberList;

    QMap<QString, GroupMemberModel*> * groupMemberMap;

    QVBoxLayout * g_layout;//群组成员列表布局

};

#endif // WECHAT_H
