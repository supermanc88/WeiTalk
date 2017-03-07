#include "singlechat.h"
#include "ui_singlechat.h"

SingleChat::SingleChat(const QString& bareJid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleChat)
{
    ui->setupUi(this);
    ui->nickname->setText(bareJid);
    this->bareJid = bareJid;

    setAttribute(Qt::WA_QuitOnClose, false);

    setWindowFlags(Qt::FramelessWindowHint);
//    ui->slideNavigation->setFixed(true);
//    ui->slideNavigation->setOrientation(Qt::Vertical);
//    ui->slideNavigation->addItem("111111");
//    ui->slideNavigation->addItem("222222");
//    ui->slideNavigation->addItem("333333");
//    ui->slideNavigation->addItem("444444");
//    ui->slideNavigation->addItem("555555");

    connect(ui->myLabel, SIGNAL(clicked()), this, SLOT(ShowMinimize()));
    connect(ui->myLabel_2, SIGNAL(clicked()), this, SLOT(CloseCurrentWindow()));
//    connect(ui->myLabel_2, SIGNAL(clicked()), this, SIGNAL(closeSingleChat()));
}

SingleChat::~SingleChat()
{
    delete ui;
}

void SingleChat::ShowMinimize()
{
    this->showMinimized();
}

void SingleChat::CloseCurrentWindow()
{
    emit closeSingleChat(this->bareJid);
    this->close();
}
