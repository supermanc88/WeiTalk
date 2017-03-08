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

signals:
    void showGroupChatDialog(QString groupName);

public slots:
    //重写双击事件，用来打开群组聊天
    virtual void mouseDoubleClickEvent(QMouseEvent * event);

private:
    Ui::GroupItem *ui;

    QString groupName;
};

#endif // GROUPITEM_H
