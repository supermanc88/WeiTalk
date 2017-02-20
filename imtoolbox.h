#ifndef IMTOOLBOX_H
#define IMTOOLBOX_H

#include <QWidget>

class QVBoxLayout;
class IMToolItem;

class IMToolBox : public QWidget
{
    Q_OBJECT
public:
    explicit IMToolBox(QWidget *parent = 0);
    void addItem(IMToolItem * item);

signals:

public slots:

private:
    QVBoxLayout * m_layout;
};

#endif // IMTOOLBOX_H
