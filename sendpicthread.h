#ifndef SENDPICTHREAD_H
#define SENDPICTHREAD_H

#include <QThread>
#include "weitalkserverapi.h"


class SendPicThread : public QThread
{
public:
    SendPicThread();

protected:
    virtual void run();

private:
    WeiTalkServerAPI WTAPI;
    bool b_stop;
};

#endif // SENDPICTHREAD_H
