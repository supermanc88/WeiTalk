#include "imtoolbox.h"
#include <QVBoxLayout>
#include "imtoolitem.h"

IMToolBox::IMToolBox(QWidget *parent) : QWidget(parent),
    m_layout(new QVBoxLayout)
{
    setLayout(m_layout);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
}

void IMToolBox::addItem(IMToolItem *item)
{
    m_layout->addWidget(item);
}
