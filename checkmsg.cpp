#include "checkmsg.h"
#include "ui_checkmsg.h"

CheckMsg::CheckMsg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckMsg)
{
    ui->setupUi(this);
}

CheckMsg::~CheckMsg()
{
    delete ui;
}
