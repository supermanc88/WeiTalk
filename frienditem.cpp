#include "frienditem.h"
#include "ui_frienditem.h"
#include <QMouseEvent>
#include <QDebug>

#include "singlechat.h"
#include "QXmppPresence.h"

#include <QMenu>

FriendItem::FriendItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendItem)
{
    ui->setupUi(this);
}

FriendItem::FriendItem(const QString nickName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendItem)
{
    ui->setupUi(this);
    ui->nickNmae->setText(nickName);

    m_chat = new QAction(tr("聊天"), this);
    m_profile = new QAction(tr("详细信息"), this);
    m_removeContact = new QAction(tr("删除好友"), this);

    connect(m_chat, SIGNAL(triggered(bool)), this, SLOT(showChatDialog_helper()));
    connect(m_profile, SIGNAL(triggered(bool)), this, SLOT(showProfile_helper()));
    connect(m_removeContact, SIGNAL(triggered(bool)), this, SLOT(removeContact_helper()));

}

FriendItem::~FriendItem()
{
    delete ui;
}

void FriendItem::setPresence(const QXmppPresence &presence)
{
    QString statusText = presence.statusText();
    if(statusText.isEmpty())
    {
        if(presence.type() == QXmppPresence::Available)
        {
            statusText = tr("Available");
        }
        else if(presence.type() == QXmppPresence::Unavailable)
        {
            statusText = tr("Offline");
        }
    }

    if(presence.type() == QXmppPresence::Available)
    {
        //setStyleSheet();
        switch (presence.availableStatusType()) {
        case QXmppPresence::Online:
            this->ui->friendStatus->setStyleSheet(QStringLiteral("image: url(:/images/imonline.png);"));
            break;
        case QXmppPresence::Chat:
            this->ui->friendStatus->setStyleSheet(QStringLiteral("image: url(:/images/imcallme.png);"));
            break;
        case QXmppPresence::Away:
            this->ui->friendStatus->setStyleSheet(QStringLiteral("image: url(:/images/imaway.png);"));
            break;
        case QXmppPresence::XA: //隐身
            this->ui->friendStatus->setStyleSheet(QStringLiteral("image: url(:/images/imonline.png);"));
            break;
        case QXmppPresence::DND:
            this->ui->friendStatus->setStyleSheet(QStringLiteral("image: url(:/images/imsilent.png);"));
            break;
        default:
            break;
        }
    }

}

void FriendItem::showChatDialog_helper()
{
    QString bareJid = this->ui->nickNmae->text();
    qDebug()<<this->ui->nickNmae->text();
//    SingleChat *a = new SingleChat;
//    a->show();
    emit showChatDialog(bareJid);
}

void FriendItem::showProfile_helper()
{
    qDebug()<<this->ui->nickNmae->text();
//    SingleChat *a = new SingleChat;
//    a->show();
    //暂无好友详细信息窗口，以聊天窗口代替
}

void FriendItem::removeContact_helper()
{
    QString bareJid = this->ui->nickNmae->text();
    if(!bareJid.isEmpty())
    {
        emit removeContact(bareJid);
    }
}

//双击打开聊天窗口
void FriendItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"double click";
    QString bareJid = this->ui->nickNmae->text();
    emit showChatDialog(bareJid);
//    SingleChat *a = new SingleChat;
//    a->show();
//    return QWidget::mouseDoubleClickEvent(event);
}

//右键Item出现菜单
void FriendItem::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::RightButton)
    {
        qDebug()<<"haoyoukuang 1111111";
        QMenu menu(this);
        menu.addAction(m_chat);
        menu.addAction(m_profile);
        menu.addAction(m_removeContact);
        menu.exec(QCursor::pos()); //菜单出现位置为鼠标当前位置
    }
//    return mousePressEvent(event);
}
