#ifndef USERORGROUPITEM_H
#define USERORGROUPITEM_H

#include <QWidget>

namespace Ui {
class UserOrGroupItem;
}

class UserOrGroupItem : public QWidget
{
    Q_OBJECT

public:
    explicit UserOrGroupItem(QWidget *parent = 0);
    ~UserOrGroupItem();
    void setNameLabelText(const QString & itemName);

    //当搜索为群组的时候，需要把头像改为群组头像，添加按钮内容改为加入
    void changeGroupSomething();

private:
    Ui::UserOrGroupItem *ui;
};

#endif // USERORGROUPITEM_H
