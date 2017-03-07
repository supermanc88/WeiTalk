#include "groupitem.h"
#include "ui_groupitem.h"
#include <QDebug>

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
