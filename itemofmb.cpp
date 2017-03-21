#include "itemofmb.h"
#include "ui_itemofmb.h"

ItemOfMB::ItemOfMB(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemOfMB)
{
    ui->setupUi(this);
}

ItemOfMB::~ItemOfMB()
{
    delete ui;
}

QString ItemOfMB::username() const
{
    QString username = ui->name->text();
    return username;
}

void ItemOfMB::setUsername(const QString &username)
{
    ui->name->setText(username);
}

int ItemOfMB::messageCount() const
{
    int messageCount = ui->number->text().toInt();
    return messageCount;
}

void ItemOfMB::setMessageCount(int messageCount)
{
    ui->number->setText(QString("%1").arg(messageCount));
}
