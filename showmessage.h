#ifndef SHOWMESSAGE_H
#define SHOWMESSAGE_H

#include <QWidget>

class ItemOfMB;
namespace Ui {
class ShowMessage;
}

class ShowMessage : public QWidget
{
    Q_OBJECT

signals:
    void cancelLightingBtn();
    void readAllBtn();

public:
    explicit ShowMessage(QWidget *parent = 0);
    ~ShowMessage();
public slots:
    void addItem(ItemOfMB * item);

private:
    Ui::ShowMessage *ui;
};

#endif // SHOWMESSAGE_H
