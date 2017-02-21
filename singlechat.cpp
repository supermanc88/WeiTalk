#include "singlechat.h"
#include "ui_singlechat.h"

SingleChat::SingleChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleChat)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    ui->slideNavigation->setFixed(true);
    ui->slideNavigation->setOrientation(Qt::Vertical);
    ui->slideNavigation->addItem("111111");
    ui->slideNavigation->addItem("222222");
    ui->slideNavigation->addItem("333333");
    ui->slideNavigation->addItem("444444");
    ui->slideNavigation->addItem("555555");
}

SingleChat::~SingleChat()
{
    delete ui;
}
