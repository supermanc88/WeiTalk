#include "grouptoolitem.h"

#include <QVBoxLayout>
#include "listbutton.h"

GroupToolItem::GroupToolItem(const QString& string, QWidget *parent) : QWidget(parent)
{
    m_btn = new ListButton(string);
    m_layout = new QVBoxLayout;

    m_itemList  = new QList<QWidget *>;

    m_isVisiable = false;

    m_layout->addWidget(m_btn);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);

    setLayout(m_layout);

    connect(m_btn, SIGNAL(click()), this, SLOT(listShowOrHide()));
}

void GroupToolItem::addItem(QWidget *item)
{
    m_itemList->append(item);

    m_layout->addWidget(item);

    if(m_itemList->size() == 0)
    {
        item->setVisible(false);
    }
    else
    {
        item->setVisible(m_isVisiable);
    }
}

GroupItem *GroupToolItem::getOrCreateItem(QString groupName)
{
    if(m_groupsMap.contains(groupName))
    {
        return m_groupsMap[groupName];
    }
    else
    {
        GroupItem * item = new GroupItem(groupName);
        m_groupsMap[groupName] = item;
        addItem(item);
        return item;
    }
}

void GroupToolItem::listShowOrHide()
{
    for(int i=0; i<m_itemList->size(); i++)
    {
        m_itemList->at(i)->setVisible(!(m_itemList->at(i)->isVisible()));
    }
}
