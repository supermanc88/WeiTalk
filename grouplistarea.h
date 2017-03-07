#ifndef GROUPLISTAREA_H
#define GROUPLISTAREA_H

#include <QWidget>
#include <QVBoxLayout>

namespace Ui {
class GroupListArea;
}

class GroupListArea : public QWidget
{
    Q_OBJECT

public:
    explicit GroupListArea(QWidget *parent = 0);
    ~GroupListArea();

    void addItem(QWidget * groupItem);

private:
    Ui::GroupListArea *ui;

    QVBoxLayout * groupItemsLayout;
};

#endif // GROUPLISTAREA_H
