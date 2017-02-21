#include "frienditem.h"
#include "ui_frienditem.h"
#include <QMouseEvent>
#include <QDebug>

#include "singlechat.h"

FriendItem::FriendItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendItem)
{
    ui->setupUi(this);
}

FriendItem::~FriendItem()
{
    delete ui;
}

//双击打开聊天窗口
void FriendItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"double click";
    SingleChat *a = new SingleChat;
    a->show();
    return QWidget::mouseDoubleClickEvent(event);
}
