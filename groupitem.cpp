#include "groupitem.h"
#include "ui_groupitem.h"
#include <QDebug>
#include <QMouseEvent>

GroupItem::GroupItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupItem)
{
    ui->setupUi(this);
    qDebug()<<"GroupItem construct";
}

GroupItem::GroupItem(QString groupName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupItem)
{
    ui->setupUi(this);
    ui->groupName->setText(groupName); //更新群名
    groupId = 0;
}

GroupItem::~GroupItem()
{
    delete ui;
}

QString GroupItem::getGroupName() const
{
    return groupName;
}

void GroupItem::setGroupName(const QString &value)
{
    groupName = value;
    ui->groupName->setText(groupName);
}

void GroupItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    QString m_groupName = getGroupName();

    emit showGroupChatDialog(m_groupName, groupId);
}

int GroupItem::getGroupId() const
{
    return groupId;
}

void GroupItem::setGroupId(int value)
{
    groupId = value;
}
