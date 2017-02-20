#include "frienddialog.h"
#include <QMouseEvent>
//#include <QtDebug>

#include <QVBoxLayout>
#include <QScrollArea>
#include <QWidget>
#include <QPushButton>
#include "friendlistarea.h"
#include "frienditem.h"
#include "imtoolbox.h"
#include "imtoolitem.h"


/*
 * 目前这样可以移动整个窗口，先做个样子，后期可以把头部单独拿出来
 * 做一个Dialog 然后重写鼠标事件，再把那个Dialog添加到本Dialog
 * 上来
 */

FriendDialog::FriendDialog(QDialog *parent) : QDialog(parent)
{
    setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint); //隐藏标题栏

    this->m_moving  = false;

    FriendListArea * m_friendListArea = new FriendListArea;
    m_friendListArea->adjustSize();

    FriendItem * a1 = new FriendItem;
    FriendItem * a2 = new FriendItem;
    FriendItem * a3 = new FriendItem;
    FriendItem * a4 = new FriendItem;
    FriendItem * a5 = new FriendItem;
    FriendItem * a6 = new FriendItem;
    FriendItem * a7 = new FriendItem;
    FriendItem * a8 = new FriendItem;
    FriendItem * a9 = new FriendItem;
    FriendItem * a10 = new FriendItem;
    FriendItem * a11 = new FriendItem;
    FriendItem * a12 = new FriendItem;
    FriendItem * a13 = new FriendItem;
    FriendItem * a14 = new FriendItem;
    FriendItem * a15 = new FriendItem;

    IMToolBox * m_box1 = new IMToolBox;
    IMToolItem * m_item1 = new IMToolItem("title1");
    m_item1->addItem(a1);
    m_item1->addItem(a2);
    m_item1->addItem(a3);
    m_item1->addItem(a4);
    m_item1->addItem(a5);
    m_item1->addItem(a6);
    m_item1->addItem(a10);
    m_item1->addItem(a11);
    m_item1->addItem(a12);
    m_item1->addItem(a13);
    m_item1->addItem(a14);
    m_item1->addItem(a15);

    m_box1->addItem(m_item1);

    IMToolBox * m_box2 = new IMToolBox;
    IMToolItem * m_item2 = new IMToolItem("title1");
    m_item2->addItem(a7);
    m_item2->addItem(a8);
    m_item2->addItem(a9);
    m_box2->addItem(m_item2);

    m_friendListArea->addItem(m_box1);
    m_friendListArea->addItem(m_box2);


    this->friendAreaLayout->addWidget(m_friendListArea);
}

void FriendDialog::mousePressEvent(QMouseEvent *event)
{
    this->m_moving = true;
    this->m_movePosition = event->globalPos() - this->pos();
    return QDialog::mousePressEvent(event);
}

void FriendDialog::mouseReleaseEvent(QMouseEvent *event)
{
    this->m_moving = false;
    return QDialog::mousePressEvent(event);
}

void FriendDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() && Qt::LeftButton &&
            (event->globalPos()-m_movePosition).manhattanLength() > QApplication::startDragDistance())
    {
        this->move(event->globalPos() - m_movePosition);
        return QDialog::mouseMoveEvent(event);
    }
}

