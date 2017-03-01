#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QPropertyAnimation>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QSequentialAnimationGroup>
#include "ui_login.h"

#include "QXmppClient.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog, public Ui::LoginDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QXmppClient *xmppClient, QWidget *parent = 0);

signals:
    void loginSucess(); //登录成功，发送此信号，显示好友界面

public slots:
    //为支持窗口无标题栏，自由拖动，重写鼠标事件
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);     //托盘双击响应

    void initSysTrayContextMenu();  //初始化设置托盘菜单

    void initLoginAnimation(); //初始化登录动画

    void on_loginPushButton_clicked(); //登录按钮

    void signIn();//登录

    void cancelSignIn(); //取消登录

    void showSignInPage(); //显示登录时的界面

    void showSignInPageAfterUserDisconnection(); //显示登录失败时或取消登录的界面

//    void rosterReceived();

private slots:
    void CloseWindow();//关闭
    void ShowMinimize();//最小化
    void startConnection();  //开始连接登录

private:
    QPoint m_movePosition;
    bool m_moving;

//    QPropertyAnimation * m_loginAnimation; //设置登录动画
    QSequentialAnimationGroup * m_loginAnimationGroup; //登录动画组

    QSystemTrayIcon * m_trayIcon; //系统托盘

    QMenu * m_sysTrayMenu; //托盘菜单

    //记录图片原始位置
    int pic_startX;
    int pic_startY;

    QXmppClient * m_xmppClient;

};

#endif // LOGINDIALOG_H
