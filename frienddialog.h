#ifndef FRIENDDIALOG_H
#define FRIENDDIALOG_H

#include <QDialog>
#include "ui_frienddialog.h"

namespace Ui {
class FriendDialog;
}

class FriendDialog : public QDialog, public Ui::FriendDialog
{
    Q_OBJECT
public:
    explicit FriendDialog(QDialog *parent = 0);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

signals:

public slots:
    void ShowMinimize();
    void CloseApp();

private:
    bool m_moving;
    QPoint m_movePosition;
};

#endif // FRIENDDIALOG_H
