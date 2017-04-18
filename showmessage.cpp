#include "showmessage.h"
#include "ui_showmessage.h"
//#include <QPushButton>
#include "itemofmb.h"

ShowMessage::ShowMessage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowMessage)
{
    ui->setupUi(this);

    connect(this->ui->cancelLighting, SIGNAL(clicked(bool)), this, SIGNAL(cancelLightingBtn()));
    connect(this->ui->readAll, SIGNAL(clicked(bool)), this, SIGNAL(readAllBtn()));
}

ShowMessage::~ShowMessage()
{
    delete ui;
}

void ShowMessage::addItem(ItemOfMB *item)
{
    int count = ui->verticalLayout->count();
    if (count > 1)
    {
        ui->verticalLayout->removeItem(ui->verticalLayout->itemAt(count - 1));
    }
    ui->verticalLayout->addWidget(item);
    ui->verticalLayout->addStretch();
}
