#include "mylabel.h"
#include <QMouseEvent>

MyLabel::MyLabel(QWidget *parent) :
    QLabel(parent)
{
}

void MyLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        emit clicked();
    }
}
