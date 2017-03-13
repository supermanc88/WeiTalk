#ifndef SINGLECHAT_H
#define SINGLECHAT_H

#include <QWidget>
#include "QXmppClient.h"

class QXmppMessage;

namespace Ui {
class SingleChat;
}

class SingleChat : public QWidget
{
    Q_OBJECT

public:
    explicit SingleChat(const QString& bareJid, QWidget *parent = 0);
    ~SingleChat();

    void setClient(QXmppClient *value);

    QString getContentOfSend() const;

signals:
    void closeSingleChat(QString bareJid);

public slots:
    void ShowMinimize();
    void CloseCurrentWindow();

    //send message to person
    void SendMessage();

    void messageReceived(const QXmppMessage & message);

private:
    Ui::SingleChat *ui;
    QString bareJid;

    QXmppClient * client;

    QString contentOfSend;
};

#endif // SINGLECHAT_H
