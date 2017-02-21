#ifndef WECHAT_H
#define WECHAT_H

#include <QWidget>

namespace Ui {
class WeChat;
}

class WeChat : public QWidget
{
    Q_OBJECT

public:
    explicit WeChat(QWidget *parent = 0);
    ~WeChat();

private:
    Ui::WeChat *ui;
};

#endif // WECHAT_H
