#ifndef IMTOOLITEM_H
#define IMTOOLITEM_H

#include <QWidget>

class QLabel;
class QVBoxLayout;
class QPushButton;
class ListButton;

class IMToolItem : public QWidget
{
    Q_OBJECT
public:
    explicit IMToolItem(const QString &title, QWidget *parent = 0);

    void addItem(QWidget * item);  //添加好友列


signals:

public slots:
    void listShowOrHide();
protected:
    //这样写这个点击事件  会使整个类全部获取点击 不可用
//    virtual void mousePressEvent(QMouseEvent * event); //重写鼠标点击事件

private:
    ListButton * m_btn;  //用来接收title  定义分组名称
    QVBoxLayout * m_layout; //好友列表布局
    bool m_isVisible; //用来下拉是否显示
    QList<QWidget *> * m_itemList;  //用来存储好友列表
};

#endif // IMTOOLITEM_H
