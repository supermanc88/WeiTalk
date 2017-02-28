#ifndef WEITALKSERVERAPI_H
#define WEITALKSERVERAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QList>

typedef struct user_info_s
{
    QString acNickName;
    QString acUserName;
    int nUserId;
}user_info_t;

class WeiTalkServerAPI : public QObject
{
    Q_OBJECT
public:
    explicit WeiTalkServerAPI(QObject *parent = 0);

    static QByteArray getHtml(QString url);
    static void SelectUserText(const QString username, QList<user_info_t> * userList);


signals:

public slots:
};

#endif // WEITALKSERVERAPI_H
