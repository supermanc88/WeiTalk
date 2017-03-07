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

GroupItem::~GroupItem()
{
    delete ui;
}
