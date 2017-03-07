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

    QString getGroupName() const;
    void setGroupName(const QString &value);

private:
    Ui::GroupItem *ui;

    QString groupName;
};

#endif // GROUPITEM_H
