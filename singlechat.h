#ifndef SINGLECHAT_H
#define SINGLECHAT_H

#include <QWidget>
#include "QXmppClient.h"

#include <qt_windows.h>
#include "sendpicthread.h"

#include "myemotion.h"

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

    //截图信号
    void captureFinished(int type);

    void insertCapture();

public slots:
    void ShowMinimize();
    void CloseCurrentWindow();

    //send message to person
    void SingleSendMessage();

    void messageReceived(const QXmppMessage & message);


    //设置聊天内容
    void setChatText(QString message);
    void setChatContent(QString message);

    //只发送图片代码
    void sendSinglePic();

    //显示表情界面
    void showEmotion();

/***************截图功能start******************************/
    void onCapture();

    //截图回调函数
    friend void CaptureNotice(int nType, int x, int y, int width, int height, const char *szInfo);

    void OnCaptureFinish(int type);

    void InsertCapture();
/***************截图功能end********************************/

    //插入表情
    void insertEmotion(int row, int column);
private:
    Ui::SingleChat *ui;
    QString bareJid;

    QXmppClient * client;

    QString contentOfSend;

    QString uploadPicPath;

    SendPicThread * SendThreadA;

    MyEmotion * emotion;
};

#endif // SINGLECHAT_H
