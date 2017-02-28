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

private:
    Ui::UserOrGroupItem *ui;
};

#endif // USERORGROUPITEM_H
