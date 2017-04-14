#ifndef MYEMOTION_H
#define MYEMOTION_H

#include <QWidget>

namespace Ui {
class MyEmotion;
}

class MyEmotion : public QWidget
{
    Q_OBJECT

public:
    explicit MyEmotion(QWidget *parent = 0);
    ~MyEmotion();

private:
    Ui::MyEmotion *ui;
};

#endif // MYEMOTION_H
