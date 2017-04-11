#include "sendpicthread.h"
#include <QXmppClient.h>

extern QString newPicPath;  //最初的定义在frienddialog
extern QXmppClient * globalClient;  //最初定义在frienddialog
extern QString currentChattingJID; //最初的定义在frienddialog

SendPicThread::SendPicThread(QObject *parent) : QThread(parent)
{
    b_stop = true;
}

void SendPicThread::run()
{
    qDebug()<<"sub thread is running";
//    while(b_stop)
//    {
        WTAPI.OnUploadPic(newPicPath);
//        QString sentHtml = QString("<!DOCTYPE HTML PUBLIC \"-/W3C/DTD HTML 4.0/EN\" \"http:/www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\"%1\" /></p></body></html>").arg(newPicPath);

//        globalClient->sendMessage(currentChattingJID, sentHtml);
//        b_stop = false;
        emit finishedUpLoadPic();
//    }

//    exec();
    qDebug()<<"sub thread is finished";
}
