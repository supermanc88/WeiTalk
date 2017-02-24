#include "imtoolitem.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include "listbutton.h"
#include <QDebug>

#include <QXmppPresence.h>

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

}

void IMToolItem::addItem(QWidget *item)
{
    m_itemList->append(item);
    m_layout->addWidget(item);

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
    FriendItem *item = getOrCreateItem(bareJid);
    if (!presences.isEmpty())
        item->setPresence(*presences.begin());
    else
        item->setPresence(QXmppPresence(QXmppPresence::Unavailable));
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
