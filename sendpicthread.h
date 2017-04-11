#ifndef SENDPICTHREAD_H
#define SENDPICTHREAD_H

#include <QThread>
#include "weitalkserverapi.h"


class SendPicThread : public QThread
{
    Q_OBJECT
signals:
    //当图片上传完毕后，发送此信号，由wechat中的sendsinglepic处理
    void finishedUpLoadPic();
public:
    SendPicThread(QObject *parent = 0);

protected:
    virtual void run();

private:
    WeiTalkServerAPI WTAPI;
    bool b_stop;
};

#endif // SENDPICTHREAD_H
