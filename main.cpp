#include <QApplication>

#include <QWidget>
#include "logindialog.h"

#include "ui_login.h"

#include "singlechat.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    LoginDialog w;
//    w.show();

    SingleChat w;
    w.show();

    return a.exec();
}
