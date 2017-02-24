#ifndef FRIENDITEM_H
#define FRIENDITEM_H

/*
 *单个好友项模板
 */

#include <QWidget>
#include <QAction>


class SingleChat;
class QXmppPresence;

namespace Ui {
class FriendItem;
}

class FriendItem : public QWidget
{
    Q_OBJECT

public:
    explicit FriendItem(QWidget *parent = 0);
    //有昵称的构造函数
    explicit FriendItem(const QString nickName, QWidget *parent = 0);
    ~FriendItem();

    //更新好友在线状态
    void setPresence(const QXmppPresence &presence);

signals:
    void removeContact(const QString &bareJid);
    void showChatDialog(const QString &bareJid);

public slots:
    void showChatDialog_helper(); //显示聊天窗口
    void showProfile_helper(); //显示好友详细信息窗口
    void removeContact_helper();//删除好友

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent * event); //重写双击事件  双击后打开聊天窗口

    virtual void mousePressEvent(QMouseEvent * event); //创建右键菜单

private:
    Ui::FriendItem *ui;
//    SingleChat

    QAction *m_chat; //聊天
    QAction *m_profile;//详细信息
    QAction *m_removeContact; //删除好友
};

#endif // FRIENDITEM_H
