﻿#ifndef FRIENDDIALOG_H
#define FRIENDDIALOG_H

#include <QDialog>
#include "ui_frienddialog.h"

#include "QXmppClient.h"
#include "logindialog.h"
#include "imtoolitem.h"
#include "weitalkserverapi.h"

#include "QXmppMucManager.h"
#include <string>
#include "MD5.h"


using namespace std;

class ShowMessage;
class FriendListArea;
class GroupListArea;
class GroupToolItem;
//class QXmppMessage;


namespace Ui {
class FriendDialog;
}

class FriendDialog : public QDialog, public Ui::FriendDialog
{
    Q_OBJECT

public:
    //定义移动方向
    enum Direction
    {
        None,
        Up,
        Right,
        Left
    };

public:
    explicit FriendDialog(QDialog *parent = 0);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);


/**********************************************************
 * 截图初始化相关函数*/
    string GenerateAuthCode(string strAuthCode);
/**********************************************************/

    ~FriendDialog();

signals:

public slots:
    void ShowMinimize();
    void CloseApp();

    void runApp();

    void showFriendDialog();

    void InitGlobalClient();

    void rosterReceived();

    //好友状态改变
    void presenceChanged(const QString& bareJid, const QString& resource);

/***************************申请好友start******************/
    //收到好友申请，并回复
    void presenceReceived(const QXmppPresence& presence);

    //添加好友
    void addContact();
/***************************申请好友end*********************/
    //删除好友
    void actionRemovecontact(const QString & bareJid);

    //显示查找好友、群组界面
    void showSearchUserOrGroup();

    //获取已经加入的群组列表
    void getGroupList();

/**************************************************
 * 出席所有房间和离开所有房间
 * 效果为群组上线和下线
 */

    void joinAllRoom();
    void leaveAllRoom();

    //所有房间的信号：参与者登录，参与者离开，有新的参与者加入，信息等等
    void allRoomEvents();


    //参与者离开
    void participantRemoved(const QString &jid);

    //参与者加入房间或者有新的参与者加入
    void participantAdded(const QString &jid);
/**************************************************/


/**************************************************/
    //功能函数
    //从服务器端传回的房间JID中截取username
    QString getUserNameFromJID(const QString &jid);

    /**
     * @brief changeParticpantState
     * @param groupId
     * @param username
     *
     * 根据房间号和用户名改变出席状态离席
     */
    void changeParticpantStateLeave(int groupId, QString username);

    /**
     * @brief changeParticpantStateAdd
     * @param groupId
     * @param username
     *
     * 根据房间号和用户名改变出席状态之出席
     *
     * 有原来人员加入和新人员加入两种情况
     */
    void changeParticpantStateAdd(int groupId, QString username);

/**************************************************/


/***************************************************
 * 窗口自动隐藏功能函数
 */

    //判断是否自动隐藏
    void isAutoHide();

    //显示隐藏的界面
    void showWidget();

    //隐藏界面
    void hideWidget();

    virtual void leaveEvent(QEvent * event);

    virtual void enterEvent(QEvent * event);

/***************************************************/

    //单人聊天列表
    void showSingleChatList();

    //群组聊天列表
    void showGroupChatList();

    //临时聊天列表
    void showTemporaryChatList();

/*****************************************************
 * message receive test*/

    //get the room config
    void configurationReceived(const QXmppDataForm &configuration);

    void messageReceived(const QXmppMessage &message);

    void messageGroupReceived(const QXmppMessage &message);

/****************************************************/


/***************************最小化到托盘start**************/

    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);     //托盘双击响应

    void initSysTrayContextMenu();  //初始化设置托盘菜单

/***************************最小化到托盘end****************/

/**************************消息盒子start*******************/
    void cancelLightingMessageBox();
    void readAllMessageBox();
/**************************消息盒子end*********************/

private:
    bool m_moving;
    QPoint m_movePosition;

    QXmppClient * m_xmppClient;

    LoginDialog * m_loginDialog;

    IMToolItem * m_item1;
    GroupToolItem * m_groupItme;

    FriendListArea * m_friendListArea;

    GroupListArea * m_groupListArea;


    Direction m_direction; //隐藏方向
    bool m_isAutoHide;//是否自动隐藏

    int m_desktopWidth; //桌面宽度


    WeiTalkServerAPI WTAPI;

    QList<group_info_t> * groupList;


    QMap<QString, QXmppMucRoom*> groupRoomMap;

    ShowMessage * m_showMessage;


    QSystemTrayIcon * m_trayIcon; //系统托盘

    QMenu * m_sysTrayMenu; //托盘菜单

};

#endif // FRIENDDIALOG_H
