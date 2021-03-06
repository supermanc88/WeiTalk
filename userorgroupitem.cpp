#include "userorgroupitem.h"
#include "ui_userorgroupitem.h"

UserOrGroupItem::UserOrGroupItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserOrGroupItem)
{
    ui->setupUi(this);
}

UserOrGroupItem::~UserOrGroupItem()
{
    delete ui;
}

void UserOrGroupItem::setNameLabelText(const QString &itemName)
{
    this->ui->nameLabel->setText(itemName);
}

void UserOrGroupItem::changeGroupSomething()
{
    this->ui->picLabel->setStyleSheet(QStringLiteral("image: url(:/images/DefGroupHeadPic.jpg);"));
    this->ui->addContactButton->setText("+加入");
}
