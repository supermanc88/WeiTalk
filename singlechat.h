#ifndef SINGLECHAT_H
#define SINGLECHAT_H

#include <QWidget>

namespace Ui {
class SingleChat;
}

class SingleChat : public QWidget
{
    Q_OBJECT

public:
    explicit SingleChat(const QString& bareJid, QWidget *parent = 0);
    ~SingleChat();

signals:
    void closeSingleChat(QString bareJid);

public slots:
    void ShowMinimize();
    void CloseCurrentWindow();

private:
    Ui::SingleChat *ui;
    QString bareJid;
};

#endif // SINGLECHAT_H
