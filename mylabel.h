/*
 * 为了使QLabel具有点击信号，顾自定义MyLabel
 *
 */

#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel
{
    Q_OBJECT

public:
    MyLabel(QWidget *parent = 0);

signals:
    void clicked();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
};

#endif
