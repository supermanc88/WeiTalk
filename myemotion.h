#ifndef MYEMOTION_H
#define MYEMOTION_H

#include <QWidget>
#include <QList>

namespace Ui {
class MyEmotion;
}

class MyEmotion : public QWidget
{
    Q_OBJECT
signals:
    void cellClicked(int row, int column);

public:
    explicit MyEmotion(QWidget *parent = 0);
    ~MyEmotion();

    void addEmotionItem(QString fileName);
    void initEmotion();

private:
    Ui::MyEmotion *ui;
    QList<QString> m_emotionList;
};

#endif // MYEMOTION_H
