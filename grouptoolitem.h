#ifndef GROUPTOOLITEM_H
#define GROUPTOOLITEM_H

#include <QWidget>

class ListButton;
class QVBoxLayout;

class GroupToolItem : public QWidget
{
    Q_OBJECT
public:
    explicit GroupToolItem(const QString& string, QWidget *parent = 0);

    void addItem(QWidget * item);

signals:

public slots:
    void listShowOrHide();

private:
    ListButton *m_btn;
    QVBoxLayout * m_layout;
    bool m_isVisiable;
    QList<QWidget *> * m_itemList;

};

#endif // GROUPTOOLITEM_H
