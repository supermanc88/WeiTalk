#include "frienditem.h"
#include "ui_frienditem.h"
#include <QMouseEvent>
#include <QDebug>

#include "singlechat.h"
#include "QXmppPresence.h"

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

//双击打开聊天窗口
void FriendItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"double click";
    SingleChat *a = new SingleChat;
    a->show();
    return QWidget::mouseDoubleClickEvent(event);
}
