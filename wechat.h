#ifndef WECHAT_H
#define WECHAT_H

#include <QWidget>
#include "weitalkserverapi.h"
#include "QXmppClient.h"

#include <qt_windows.h>

#include "weitalkserverapi.h"

class QVBoxLayout;
class GroupMemberModel;

namespace Ui {
class WeChat;
}

class WeChat : public QWidget
{
    Q_OBJECT

signals:

    //截图信号
    void captureFinished(int type);

    void insertCapture();

    void uploadPic(); //信号传递给主线程，由主线程发送图片

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


    //groupid  to groupJID
    QString getGroupJID();

public slots:
    //sendmessage to a room
    void sendMessage();

    void setChatContent(QString message);

/***************截图功能start******************************/
    void onCapture();

    //截图回调函数
    friend void CaptureNoticeWeChat(int nType, int x, int y, int width, int height, const char *szInfo);

    void OnCaptureFinish(int type);

    void InsertCapture();
/***************截图功能end********************************/

private:
    Ui::WeChat *ui;
    int groupId;
    QString groupName;

    int memberCount;

    WeiTalkServerAPI API;

    QList<group_member_t> * groupMemberList;

    QMap<QString, GroupMemberModel*> * groupMemberMap;

    QVBoxLayout * g_layout;//群组成员列表布局

    QXmppClient * client;

    QString uploadPicPath;

    WeiTalkServerAPI WTAPI;

};

#endif // WECHAT_H
