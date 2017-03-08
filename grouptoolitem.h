#ifndef GROUPTOOLITEM_H
#define GROUPTOOLITEM_H

#include <QWidget>
#include "groupitem.h"
#include <QMap>

class ListButton;
class QVBoxLayout;

class GroupToolItem : public QWidget
{
    Q_OBJECT
public:
    explicit GroupToolItem(const QString& string, QWidget *parent = 0);

    void addItem(QWidget * item);

    GroupItem * getOrCreateItem(QString groupName, int groupId);


signals:

public slots:
    void listShowOrHide();

    void showGroupChatDialog(QString groupName, int groupId);

private:
    ListButton *m_btn;
    QVBoxLayout * m_layout;
    bool m_isVisiable;
    QList<QWidget *> * m_itemList;  //存储群组列表

    QMap<QString, GroupItem *> m_groupsMap;

};

#endif // GROUPTOOLITEM_H
