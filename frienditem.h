#ifndef FRIENDITEM_H
#define FRIENDITEM_H

/*
 *单个好友项模板
 */

#include <QWidget>

class SingleChat;

namespace Ui {
class FriendItem;
}

class FriendItem : public QWidget
{
    Q_OBJECT

public:
    explicit FriendItem(QWidget *parent = 0);
    ~FriendItem();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent * event); //重写双击事件  双击后打开聊天窗口

private:
    Ui::FriendItem *ui;
//    SingleChat
};

#endif // FRIENDITEM_H
