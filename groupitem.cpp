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
    ui->label_2->setText(groupName); //更新群名
}

GroupItem::~GroupItem()
{
    delete ui;
}
