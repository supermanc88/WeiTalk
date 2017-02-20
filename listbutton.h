#ifndef LISTBUTTON_H
#define LISTBUTTON_H

#include <QWidget>

class QLabel;
class QHBoxLayout;

class ListButton : public QWidget
{
    Q_OBJECT
public:
    explicit ListButton(const QString &title, QWidget *parent = 0);

signals:
    void click();  //给自定义的button创建click信号

public slots:

protected:
    virtual void mousePressEvent(QMouseEvent * event); //重写鼠标点击事件，用来发送click信号

private:
    QLabel * head;
    QLabel * title;
    QHBoxLayout *layout;
};

#endif // LISTBUTTON_H
