#include "myemotion.h"
#include "ui_myemotion.h"

MyEmotion::MyEmotion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyEmotion)
{
    ui->setupUi(this);
}

MyEmotion::~MyEmotion()
{
    delete ui;
}
