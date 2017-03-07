#include "grouplistarea.h"
#include "ui_grouplistarea.h"
#include <QVBoxLayout>

GroupListArea::GroupListArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupListArea)
{
    ui->setupUi(this);

    groupItemsLayout = new QVBoxLayout;

    groupItemsLayout->setContentsMargins(0,0,0,0);
    groupItemsLayout->setSpacing(0);

    ui->scrollAreaWidgetContents->setLayout(groupItemsLayout);

}

GroupListArea::~GroupListArea()
{
    delete ui;
}

void GroupListArea::addItem(QWidget *groupItem)
{
    int count = groupItemsLayout->count();

    if(count > 1)
    {
        groupItemsLayout->removeItem(groupItemsLayout->itemAt(count - 1));
    }

    groupItemsLayout->addWidget(groupItem);
    groupItemsLayout->addStretch();
}
