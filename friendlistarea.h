#ifndef FRIENDLISTAREA_H
#define FRIENDLISTAREA_H

#include <QWidget>

namespace Ui {
class FriendListArea;
}

class QVBoxLayout;

class FriendListArea : public QWidget
{
    Q_OBJECT

public:
    explicit FriendListArea(QWidget *parent = 0);
    ~FriendListArea();

    void addItem(QWidget *friendItem);

private:
    Ui::FriendListArea *ui;
    QVBoxLayout * mainLayout;
    QVBoxLayout * itemsLayout;
};

#endif // FRIENDLISTAREA_H
