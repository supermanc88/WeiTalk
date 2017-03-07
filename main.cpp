#include <QApplication>

#include <QWidget>
//#include "logindialog.h"

#include "frienddialog.h"
#include <QDebug>

//#include "groupitem.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    LoginDialog w;
//    w.show();

    FriendDialog w;
    w.runApp();

//    GroupItem w;
//    w.show();

    return a.exec();
}
