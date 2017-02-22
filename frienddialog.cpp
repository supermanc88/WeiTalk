#include "frienddialog.h"
#include <QMouseEvent>
#include <QtDebug>

#include <QVBoxLayout>
#include <QScrollArea>
#include <QWidget>
#include <QPushButton>
#include "friendlistarea.h"
#include "frienditem.h"
#include "imtoolbox.h"
#include "imtoolitem.h"

#include "QXmppRosterManager.h"


/*
 * 目前这样可以移动整个窗口，先做个样子，后期可以把头部单独拿出来
 * 做一个Dialog 然后重写鼠标事件，再把那个Dialog添加到本Dialog
 * 上来
 */

/*
 * FriendDialog构造函数
 *  首先此Dialog不显示，先显示登录Dialog
 *  确认登录成功之后，显示登录界面
 */

FriendDialog::FriendDialog(QDialog *parent) : QDialog(parent)
{
    setupUi(this);

    m_xmppClient = new QXmppClient;

    setAttribute(Qt::WA_QuitOnClose, true);

    setWindowFlags(Qt::FramelessWindowHint); //隐藏标题栏

//    this->hide();

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

    connect(this->myLabel, SIGNAL(clicked()), this, SLOT(ShowMinimize()));
    connect(this->myLabel_2, SIGNAL(clicked()), this, SLOT(close()));

    //接收好友等信息
    connect(&this->m_xmppClient->rosterManager(), SIGNAL(rosterReceived()), this, SLOT(rosterReceived()));

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

void FriendDialog::ShowMinimize()
{
    this->showMinimized();
}

void FriendDialog::CloseApp()
{

}


/*
 * 所有程序由此开始
 * 先不显示FriendDlg
 * 显示loginDlg
 */
void FriendDialog::runApp()
{
    m_loginDialog = new LoginDialog(this->m_xmppClient);
    m_loginDialog->show();
    connect(this->m_loginDialog, SIGNAL(loginSucess()), this, SLOT(show()));
}

void FriendDialog::rosterReceived()
{
    foreach (const QString &bareJid, this->m_xmppClient->rosterManager().getRosterBareJids()) {
        QString name = this->m_xmppClient->rosterManager().getRosterEntry(bareJid).name();
        if(name.isEmpty())
            name = "-";
        qDebug("example_2_rosterHandling:: Roster received: %s [%s]", bareJid.toStdString().c_str(), qPrintable(name));
    }
}

