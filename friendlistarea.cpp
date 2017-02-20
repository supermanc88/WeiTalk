#include "friendlistarea.h"
#include "ui_friendlistarea.h"
#include <QVBoxLayout>

FriendListArea::FriendListArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendListArea)
{
    ui->setupUi(this);
    mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    itemsLayout = new QVBoxLayout;
    itemsLayout->setContentsMargins(0,0,0,0);
    itemsLayout->setSpacing(0);

    ui->m_scrollAreaWidgetContents->setLayout(itemsLayout);

}

FriendListArea::~FriendListArea()
{
    delete ui;
}


//#include "friendlistarea.h"
//#include <QScrollArea>


//FriendListArea::FriendListArea(QWidget *parent) :
//    QWidget(parent)
//{
////    setupUi(this);

//    mainLayout = new QVBoxLayout;
//    mainLayout->setContentsMargins(0,0,0,0);
//    mainLayout->setSpacing(0);

//    itmesLayout = new QVBoxLayout;
//    itmesLayout->setContentsMargins(0,0,0,0);
//    itmesLayout->setSpacing(0);

//    m_scrollWidget = new QWidget;
//    m_scrollWidget->setLayout(itmesLayout);

//    m_scrollArea = new QScrollArea(this);
//    m_scrollArea->setWidget(m_scrollWidget);
//    m_scrollArea->setAlignment(Qt::AlignLeft);
//    m_scrollArea->setWidgetResizable(true);
////    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

//    m_scrollArea->show();
//    mainLayout->addWidget(m_scrollWidget);

//    setLayout(mainLayout);


//}

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
