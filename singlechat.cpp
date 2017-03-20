#include "singlechat.h"
#include "ui_singlechat.h"
#include "QXmppMessage.h"

SingleChat::SingleChat(const QString& bareJid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleChat)
{
    ui->setupUi(this);
    ui->nickname->setText(bareJid);
    this->bareJid = bareJid;

    setAttribute(Qt::WA_QuitOnClose, false);

    setWindowFlags(Qt::FramelessWindowHint);

    connect(ui->myLabel, SIGNAL(clicked()), this, SLOT(ShowMinimize()));
    connect(ui->myLabel_2, SIGNAL(clicked()), this, SLOT(CloseCurrentWindow()));

    //connect btn to send message
    connect(ui->sendMessageBtn, SIGNAL(clicked(bool)), this, SLOT(SendMessage()));


    //connect client to receive some message
//    connect(this->client, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(messageReceived(QXmppMessage)));

}

SingleChat::~SingleChat()
{
    delete ui;
}

void SingleChat::ShowMinimize()
{
    this->showMinimized();
}

void SingleChat::CloseCurrentWindow()
{
    emit closeSingleChat(this->bareJid);
    this->close();
}

void SingleChat::SendMessage()
{
    QString sendString = this->ui->textArea->text();
    client->sendMessage(this->bareJid,sendString);
    this->ui->textArea->setText("");
}

void SingleChat::messageReceived(const QXmppMessage &message)
{
    qDebug()<<message.body();
}

void SingleChat::setChatContent(QString message)
{
    ui->textBrowser->insertPlainText(message);
}

QString SingleChat::getContentOfSend() const
{
//    contentOfSend = this->ui->sendTextArea->
//    this->contentOfSend = ui->textArea->text();
    return contentOfSend;
}

void SingleChat::setClient(QXmppClient *value)
{
    client = value;
}
