#include "imtoolitem.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include "listbutton.h"
#include <QDebug>
#include "QXmppClient.h"
#include <QXmppPresence.h>
#include "singlechat.h"

QMap<QString, SingleChat *> openSingleChatMap;
extern QXmppClient * globalClient;

IMToolItem::IMToolItem(const QString &title, QWidget *parent) : QWidget(parent)
{
    m_btn = new ListButton(title);
    m_layout = new QVBoxLayout;

    m_itemList = new QList<QWidget *>;

    m_isVisible = false;  //默认不显示

    m_layout->addWidget(m_btn);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);

    setLayout(m_layout);

    connect(this->m_btn, SIGNAL(click()), this, SLOT(listShowOrHide()));
    connect(this, SIGNAL(showChatDialog(QString)), this, SLOT(showItemChatDialog(QString)));
}

void IMToolItem::addItem(QWidget *item)
{
    m_itemList->append(item);

    //绑定删除好友信号
    connect(item, SIGNAL(removeContact(QString)), this, SIGNAL(removeContact(QString)));
    //绑定打开聊天窗口信号
    connect(item, SIGNAL(showChatDialog(QString)), this, SIGNAL(showChatDialog(QString)));

    m_layout->addWidget(item);

    qDebug()<<"add item...";

    if(m_itemList->size() == 0)
    {
        item->setVisible(false);
    }
    else
    {
        item->setVisible(m_isVisible);
    }
}

void IMToolItem::listShowOrHide()
{
    qDebug()<<"1111111111";
//    if(event->button() == Qt::LeftButton)
//    {
        for(int i = 0; i < m_itemList->size(); i++)
        {
            m_itemList->at(i)->setVisible(!(m_itemList->at(i)->isVisible()));
        }

        //    }
}

FriendItem *IMToolItem::getOrCreateItem(const QString &bareJid)
{
    if(m_jidRosterItemMap.contains(bareJid))
    {
        return m_jidRosterItemMap[bareJid];
    }
    else
    {
        qDebug()<<"received:"<<bareJid;
        FriendItem * item = new FriendItem(bareJid);
        m_jidRosterItemMap[bareJid] = item;
        addItem(item);
        return item;

    }
}

FriendItem *IMToolItem::getRosterItemFromBareJid(const QString &bareJid)
{
    if(m_jidRosterItemMap.contains(bareJid))
        return m_jidRosterItemMap[bareJid];
    else
        return 0;
}

void IMToolItem::updatePresence(const QString &bareJid, const QMap<QString, QXmppPresence> &presences)
{
    FriendItem *item;
    if(!bareJid.contains("conference.im.weitainet.com", Qt::CaseSensitive))
    {
        item = getOrCreateItem(bareJid);


        if (!presences.isEmpty())
            item->setPresence(*presences.begin());
        else
            item->setPresence(QXmppPresence(QXmppPresence::Unavailable));
    }
}

void IMToolItem::showItemChatDialog(const QString &bareJid)
{
    qDebug()<<"the single chat barejid is: "<<bareJid;
    if(openSingleChatMap.contains(bareJid))
    {
        SingleChat * singleChat = openSingleChatMap[bareJid];
        singleChat->show();
        singleChat->raise();
        singleChat->activateWindow();
    }
    else
    {
        SingleChat * singleChat = new SingleChat(bareJid);
        openSingleChatMap[bareJid] = singleChat;
        singleChat->setClient(globalClient);

        connect(singleChat, SIGNAL(closeSingleChat(QString)), this, SLOT(removeItemFromChatMap(QString)));

        singleChat->show();
        singleChat->raise();
        singleChat->activateWindow();
    }
}

void IMToolItem::removeItemFromChatMap(QString bareJid)
{
    openSingleChatMap.remove(bareJid);
}

//void IMToolItem::mousePressEvent(QMouseEvent *event)
//{
//    if(event->button() == Qt::LeftButton)
//    {
//        for(int i = 0; i < m_itemList->size(); i++)
//        {
//            m_itemList->at(i)->setVisible(!(m_itemList->at(i)->isVisible()));
//        }

//    }
//    return QWidget::mousePressEvent(event);
//}
