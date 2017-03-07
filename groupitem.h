#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <QWidget>

namespace Ui {
class GroupItem;
}

class GroupItem : public QWidget
{
    Q_OBJECT

public:
    explicit GroupItem(QWidget *parent = 0);
    explicit GroupItem(QString groupName, QWidget *parent = 0);
    ~GroupItem();

private:
    Ui::GroupItem *ui;
};

#endif // GROUPITEM_H
