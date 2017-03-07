#include "friendlistarea.h"
#include "ui_friendlistarea.h"
#include <QVBoxLayout>

FriendListArea::FriendListArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendListArea)
{
    ui->setupUi(this);
//    mainLayout = new QVBoxLayout;
//    mainLayout->setContentsMargins(0,0,0,0);
//    mainLayout->setSpacing(0);

    itemsLayout = new QVBoxLayout;
    itemsLayout->setContentsMargins(0,0,0,0);
    itemsLayout->setSpacing(0);

    ui->m_scrollAreaWidgetContents->setLayout(itemsLayout);

}

FriendListArea::~FriendListArea()
{
    delete ui;
}



void FriendListArea::addItem(QWidget *friendItem)
{
    int count = itemsLayout->count();
    if (count > 1)
    {
        itemsLayout->removeItem(itemsLayout->itemAt(count - 1));
    }
    itemsLayout->addWidget(friendItem);
    itemsLayout->addStretch();
}
