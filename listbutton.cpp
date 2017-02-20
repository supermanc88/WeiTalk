#include "listbutton.h"
#include <QLabel>
#include <QHBoxLayout>

ListButton::ListButton(const QString &title, QWidget *parent) : QWidget(parent)
{
    this->title = new QLabel(title);

    this->head = new QLabel;
    head->setText("+");

    layout = new QHBoxLayout;

    layout->addWidget(head);
    layout->addWidget(this->title);
    layout->setSpacing(0);
    layout->setContentsMargins(5,5,5,5);
    layout->setMargin(5);

    this->setLayout(layout);
}

void ListButton::mousePressEvent(QMouseEvent *event)
{
    emit click();
    if(head->text() == "+")
    {
        head->setText("-");
    }
    else
    {
        head->setText("+");
    }
}
