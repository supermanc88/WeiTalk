#include <QApplication>

#include <QWidget>
//#include "logindialog.h"

#include "frienddialog.h"
#include <QDebug>

//#include "groupitem.h"

#include "myemotion.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    LoginDialog w;
//    w.show();

/***********聊天入口start*************/
    FriendDialog w;
    w.runApp();
/***********聊天入口end***************/

//    GroupItem w;
//    w.show();

/***********表情测试start*************/
//    MyEmotion w;
//    w.show();
//    w.initEmotion();
/************表情测试end**************/

    return a.exec();
}
