#ifndef FRIENDDIALOG_H
#define FRIENDDIALOG_H

#include <QDialog>
#include "ui_frienddialog.h"

#include "QXmppClient.h"
#include "logindialog.h"
#include "imtoolitem.h"

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

signals:

public slots:
    void ShowMinimize();
    void CloseApp();

    void runApp();

    void rosterReceived();

    //好友状态改变
    void presenceChanged(const QString& bareJid, const QString& resource);

    //好友申请
    void presenceReceived(const QXmppPresence& presence);

    //添加好友
    void addContact();

    //删除好友
    void actionRemovecontact(const QString & bareJid);

    //显示查找好友、群组界面
    void showSearchUserOrGroup();


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

private:
    bool m_moving;
    QPoint m_movePosition;

    QXmppClient * m_xmppClient;

    LoginDialog * m_loginDialog;

    IMToolItem * m_item1;


    Direction m_direction; //隐藏方向
    bool m_isAutoHide;//是否自动隐藏

    int m_desktopWidth; //桌面宽度
};

#endif // FRIENDDIALOG_H
