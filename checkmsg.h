#ifndef CHECKMSG_H
#define CHECKMSG_H

#include <QWidget>

namespace Ui {
class CheckMsg;
}

class CheckMsg : public QWidget
{
    Q_OBJECT

public:
    explicit CheckMsg(QWidget *parent = 0);
    ~CheckMsg();

private:
    Ui::CheckMsg *ui;
};

#endif // CHECKMSG_H
