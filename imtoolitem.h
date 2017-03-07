#ifndef IMTOOLITEM_H
#define IMTOOLITEM_H

#include <QWidget>
#include <QMap>


/*
 * 此类用于管理FriendItem
 */
#include "frienditem.h"

class QLabel;
class QVBoxLayout;
class QPushButton;
class ListButton;
class SingleChat;


class IMToolItem : public QWidget
{
    Q_OBJECT
public:
    explicit IMToolItem(const QString &title, QWidget *parent = 0);

    void addItem(QWidget * item);  //添加好友列


signals:
    //用来和单个item信号绑定，最终在friendDialog中看到此信号
    void removeContact(const QString &bareJid);
    void showChatDialog(const QString &bareJid);

public slots:
    void listShowOrHide();

    FriendItem * getOrCreateItem(const QString& bareJid);
    FriendItem * getRosterItemFromBareJid(const QString& bareJid);

    void updatePresence(const QString& bareJid, const QMap<QString, QXmppPresence>& presences);

    void showItemChatDialog(const QString &bareJid);

    void removeItemFromChatMap(QString bareJid);

protected:
    //这样写这个点击事件  会使整个类全部获取点击 不可用
//    virtual void mousePressEvent(QMouseEvent * event); //重写鼠标点击事件

private:
    ListButton * m_btn;  //用来接收title  定义分组名称
    QVBoxLayout * m_layout; //好友列表布局
    bool m_isVisible; //用来下拉是否显示
    QList<QWidget *> * m_itemList;  //用来存储好友列表


    //使用QMap存储好友列表吧，便于后面的搜索
    QMap<QString, FriendItem *> m_jidRosterItemMap;

    QMap<QString, SingleChat *> m_singleChatMap;
};

#endif // IMTOOLITEM_H
