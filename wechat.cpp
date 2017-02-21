#include "wechat.h"
#include "ui_wechat.h"

WeChat::WeChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeChat)
{
    ui->setupUi(this);
}

WeChat::~WeChat()
{
    delete ui;
}
