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
//#include "imtoolitem.h"
#include <QMessageBox>
#include <QDesktopWidget>

#include "QXmppRosterManager.h"
#include <QInputDialog>

#include "QXmppRosterIq.h"


#include "searchwidget.h"

#include "groupitem.h"
#include "grouptoolitem.h"
#include "grouplistarea.h"


extern QString LoginUserName;

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

    //创建XmppClient
    m_xmppClient = new QXmppClient;

    setAttribute(Qt::WA_QuitOnClose, true);

    /*
     * 隐藏标题栏和保持在最上
     * 保持在最上是因为靠边收起后，还可以激活
     */
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);


    groupList = new QList<group_info_t>;

//    this->hide();

    //获取桌面宽度
    m_desktopWidth = QApplication::desktop()->width();

    //隐藏方向设置空
    m_direction = None;

    //自动隐藏默认否
    m_isAutoHide = false;

    this->m_moving  = false;

    m_friendListArea = new FriendListArea;
    m_friendListArea->adjustSize();

    m_groupListArea = new GroupListArea;

    /********************************************************************************************/
    IMToolBox * m_box2 = new IMToolBox;
    m_groupItme = new GroupToolItem("我的群组");
    m_box2->addItem(m_groupItme);

    this->m_groupListArea->addItem(m_box2);
    this->groupAreaLayout->addWidget(m_groupListArea);

    /*******************************************************************************************/

    IMToolBox * m_box1 = new IMToolBox;
    m_item1 = new IMToolItem("title1");

    m_box1->addItem(m_item1);

    m_friendListArea->addItem(m_box1);

    //初始化显示好友
    this->stackedWidget->setCurrentIndex(0);
    this->myLabel_3->setStyleSheet(QStringLiteral("image: url(:/images/icon_contacts_selected.png);"));

    this->friendAreaLayout->addWidget(m_friendListArea);

    connect(this->myLabel, SIGNAL(clicked()), this, SLOT(ShowMinimize()));
    connect(this->myLabel_2, SIGNAL(clicked()), this, SLOT(close()));

    //接收好友等信息
    connect(&this->m_xmppClient->rosterManager(), SIGNAL(rosterReceived()), this, SLOT(rosterReceived()));

    //好友在线状态改变
    connect(&this->m_xmppClient->rosterManager(), SIGNAL(presenceChanged(QString,QString)), this, SLOT(presenceChanged(QString,QString)));

    //好友申请
    connect(this->m_xmppClient, SIGNAL(presenceReceived(QXmppPresence)), this, SLOT(presenceReceived(QXmppPresence)));

    //添加好友
    connect(this->pushButton_addContact, SIGNAL(clicked(bool)), this, SLOT(addContact()));

    //删除好友
    connect(this->m_item1, SIGNAL(removeContact(QString)), this, SLOT(actionRemovecontact(QString)));

    //显示查找好友群组界面
    connect(this->searchButton, SIGNAL(clicked()), this, SLOT(showSearchUserOrGroup()));

    //登录连接后获取群组列表
    connect(this->m_xmppClient, SIGNAL(connected()), this, SLOT(getGroupList()));

    /*************关联单聊、群聊、历史聊天三个按钮******************/

    connect(this->myLabel_3, SIGNAL(clicked()), this, SLOT(showSingleChatList()));

    connect(this->myLabel_4, SIGNAL(clicked()), this, SLOT(showGroupChatList()));

    connect(this->myLabel_5, SIGNAL(clicked()), this, SLOT(showTemporaryChatList()));

    /**********************************************/
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

        m_item1->getOrCreateItem(bareJid);
    }

}

void FriendDialog::presenceChanged(const QString &bareJid, const QString &resource)
{
    QMap<QString, QXmppPresence> presences = m_xmppClient->rosterManager().getAllPresencesForBareJid(bareJid);

    this->m_item1->updatePresence(bareJid, presences);
}

void FriendDialog::presenceReceived(const QXmppPresence &presence)
{
    QString reqFrom = presence.from(); //请求来源

    qDebug()<<reqFrom<<"请求";

    QString message;

    int retButton = 0;

    switch (presence.type())
    {
    case QXmppPresence::Subscribe:
        message = "<B>%1</B> wants to subscribe";

        retButton = QMessageBox::question(
                this, "Contact Subscription", message.arg(reqFrom),
                QMessageBox::Yes, QMessageBox::No);

        if(retButton == QMessageBox::Yes)
        {
            QXmppPresence subscribed;
            subscribed.setTo(reqFrom);
            subscribed.setType(QXmppPresence::Subscribed);
            m_xmppClient->sendPacket(subscribed);

//            // reciprocal subscription
//            QXmppPresence subscribe;
//            subscribe.setTo(reqFrom);
//            subscribe.setType(QXmppPresence::Subscribe);
//            m_xmppClient->sendPacket(subscribe);
        }
        else if(retButton == QMessageBox::No)
        {
            QXmppPresence unsubscribed;
            unsubscribed.setTo(reqFrom);
            unsubscribed.setType(QXmppPresence::Unsubscribed);
            m_xmppClient->sendPacket(unsubscribed);
        }

        break;

    case QXmppPresence::Subscribed:
        message = "<B>%1</B> accepted your request";
        QMessageBox::information(this, "Contact Subscription", message.arg(reqFrom),
                                 QMessageBox::Yes);
        break;
    case QXmppPresence::Unsubscribe:
        message = "<B>%1</B> unsubscribe";
        qDebug()<<message;
        break;
    case QXmppPresence::Unsubscribed:
        message = "<B>%1</B> unsubscribed";
        qDebug()<<message;
        break;

    default:
        break;
    }
}

void FriendDialog::addContact()
{
    bool ok;
    QString bareJid = QInputDialog::getText(this, "Add a jabber contact",
                                            "Contact ID:", QLineEdit::Normal, "", &ok);

    if(!ok)
        return;

//    if(!isValidBareJid(bareJid))
//    {
//        QMessageBox::information(this, "Invalid ID", "Specified ID <I>"+bareJid + " </I> is invalid.");
//        return;
//    }

    if(ok && !bareJid.isEmpty())
    {
        QXmppPresence subscribe;
        subscribe.setTo(bareJid);
        subscribe.setType(QXmppPresence::Subscribe);
        m_xmppClient->sendPacket(subscribe);
    }
}

void FriendDialog::actionRemovecontact(const QString &bareJid)
{
    int answer = QMessageBox::question(this, "Remove contact",
                                                        QString("Do you want to remove the contact <I>%1</I>").arg(bareJid),
                                                        QMessageBox::Yes, QMessageBox::No);
    if(answer == QMessageBox::Yes)
    {
        QXmppRosterIq remove;
        remove.setType(QXmppIq::Set);
        QXmppRosterIq::Item itemRemove;
        itemRemove.setSubscriptionType(QXmppRosterIq::Item::Remove);
        itemRemove.setBareJid(bareJid);
        remove.addItem(itemRemove);

        m_xmppClient->sendPacket(remove);
    }

}

void FriendDialog::showSearchUserOrGroup()
{
    SearchWidget * searchWidget = new SearchWidget;
    searchWidget->show();
}

void FriendDialog::getGroupList()
{
    qDebug()<<LoginUserName<<"username................";
    WTAPI.SelectGroupList(LoginUserName, groupList);

    QList<group_info_t>::iterator group;
    for(group=groupList->begin(); group!=groupList->end(); group++)
    {
        m_groupItme->getOrCreateItem((*group).acGroupName);
    }
}

/*
 * 根据应用程序当前位置判断朝什么方向隐藏
 */
void FriendDialog::isAutoHide()
{
    QPoint pos = this->pos();

    m_isAutoHide = true;

    if(pos.x() < 2)
    {
        m_direction = Left;
        qDebug()<<"direction left";
    }
    else if(pos.y() < 2)
    {
        m_direction = Up;
        qDebug()<<"direction up";
    }
    else if(pos.x() + this->width() > m_desktopWidth - 2)
    {
        //鼠标当前位置加上应用程序宽度超过桌面宽度，
        //说明朝右隐藏
        m_direction = Right;
        qDebug()<<"direction rigth";
    }
    else
    {
        m_direction = None;
        m_isAutoHide = false;
    }

}

void FriendDialog::showWidget()
{
    QPoint pos = this->pos();

    QPropertyAnimation * animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(100);
    animation->setStartValue(QRect(pos, this->size()));

    QRect rcEnd;

    if(m_direction == Up)
    {
        rcEnd = QRect(this->x(), 0, this->size().width(), this->rect().height());
    }
    else if (m_direction == Left)
    {
        rcEnd = QRect(0, this->y(), this->size().width(), this->rect().height());
    }
    else if (m_direction == Right)
    {
        rcEnd = QRect(m_desktopWidth - this->width(), this->y(), this->size().width(), this->rect().height());
    }

    animation->setEndValue(rcEnd);
    animation->start();
}

/*
 * 采用动画方式移动界面，造成向上收起的效果
 */
void FriendDialog::hideWidget()
{
    QPropertyAnimation * animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(100);
    animation->setStartValue(QRect(this->pos(), this->size()));

    QRect rcEnd;
    if(m_direction == Up)
    {
        rcEnd = QRect(this->x(), -this->height() + 2, this->size().width(), this->size().height());
    }
    else if (m_direction == Left)
    {
        rcEnd = QRect(-this->size().width() + 2, this->y(), this->size().width(), this->size().height());
    }
    else if (m_direction == Right)
    {
        rcEnd = QRect(m_desktopWidth - 2, this->y(), this->size().width(), this->size().height());
    }

    animation->setEndValue(rcEnd);
    animation->start();
}

void FriendDialog::leaveEvent(QEvent *event)
{
    qDebug()<<"leave event";
    isAutoHide();
    if(m_isAutoHide)
    {
        hideWidget();
    }
}

void FriendDialog::enterEvent(QEvent *event)
{
    qDebug()<<"enter event";
    if(m_isAutoHide)
    {
        showWidget();
    }
}

void FriendDialog::showSingleChatList()
{

    this->stackedWidget->setCurrentIndex(0);
    this->myLabel_3->setStyleSheet(QStringLiteral("image: url(:/images/icon_contacts_selected.png);"));
    this->myLabel_4->setStyleSheet(QStringLiteral("image: url(:/images/icon_group_normal.png);"));
    this->myLabel_5->setStyleSheet(QStringLiteral("image: url(:/images/icon_last_normal.png);"));

}

void FriendDialog::showGroupChatList()
{
    this->stackedWidget->setCurrentIndex(1);
    this->myLabel_3->setStyleSheet(QStringLiteral("image: url(:/images/icon_contacts_normal.png);"));
    this->myLabel_4->setStyleSheet(QStringLiteral("image: url(:/images/icon_group_selected.png);"));
    this->myLabel_5->setStyleSheet(QStringLiteral("image: url(:/images/icon_last_normal.png);"));


}

void FriendDialog::showTemporaryChatList()
{
    this->stackedWidget->setCurrentIndex(2);
    this->myLabel_3->setStyleSheet(QStringLiteral("image: url(:/images/icon_contacts_normal.png);"));
    this->myLabel_4->setStyleSheet(QStringLiteral("image: url(:/images/icon_group_normal.png);"));
    this->myLabel_5->setStyleSheet(QStringLiteral("image: url(:/images/icon_last_selected.png);"));
}

