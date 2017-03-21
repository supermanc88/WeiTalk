#ifndef ITEMOFMB_H
#define ITEMOFMB_H

#include <QWidget>

namespace Ui {
class ItemOfMB;
}

class ItemOfMB : public QWidget
{
    Q_OBJECT

public:
    explicit ItemOfMB(QWidget *parent = 0);
    ~ItemOfMB();

    QString username() const;
    void setUsername(const QString &username);

    int messageCount() const;
    void setMessageCount(int messageCount);

private:
    Ui::ItemOfMB *ui;

};

#endif // ITEMOFMB_H
