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

private:
    bool m_moving;
    QPoint m_movePosition;

    QXmppClient * m_xmppClient;

    LoginDialog * m_loginDialog;

    IMToolItem * m_item1;


};

#endif // FRIENDDIALOG_H
