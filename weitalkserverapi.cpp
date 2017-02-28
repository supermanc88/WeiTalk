#include "weitalkserverapi.h"
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariantMap>
#include <QDebug>


WeiTalkServerAPI::WeiTalkServerAPI(QObject *parent) : QObject(parent)
{

}

QByteArray WeiTalkServerAPI::getHtml(QString url)
{
    QNetworkAccessManager * manager = new QNetworkAccessManager;
    QNetworkReply * reply = manager->get(QNetworkRequest(QUrl(url)));

    QByteArray responseData;
    QEventLoop eventLoop;

    connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    responseData = reply->readAll();

    return responseData;
}

void WeiTalkServerAPI::SelectUserText(const QString username, QList<user_info_t> *userList)
{
    QString url = "http://api.weitainet.com/router/rest?method=weitai.user.getUserList&username=%1";

    QByteArray responseData = getHtml(url.arg(username));

    QJsonDocument d = QJsonDocument::fromJson(responseData);
    QJsonObject obj = d.object();
    if(obj.contains("rows"))
    {
        if(obj["rows"].isArray())
        {
            QJsonArray rows = obj["rows"].toArray();

            if(rows.size() == 0)
            {
                return;
            }

            for(int i=0; i<rows.size(); i++)
            {
                user_info_t userInfo;

                if(rows[i].isObject())
                {
                    QJsonObject subObj = rows[i].toObject();
                    if(subObj.contains("model"))
                    {
                        QJsonObject subSubObj = subObj["model"].toObject();
                        if(subSubObj.contains("username"))
                        {
                            QVariantMap objMap = subSubObj.toVariantMap();
                            qDebug()<<objMap["username"].toString();
                            userInfo.acUserName = objMap["username"].toString();
                        }
                    }
                    if(subObj.contains("modelext"))
                    {
                        QJsonObject subSubObj = subObj["modelext"].toObject();
                        if(subSubObj.contains("nickname"))
                        {
                            QVariantMap objMap = subSubObj.toVariantMap();
                            qDebug()<<objMap["nickname"].toString();
                            userInfo.acNickName = objMap["nickname"].toString();
                        }
                        if(subSubObj.contains("userid"))
                        {
                            QVariantMap objMap = subSubObj.toVariantMap();
                            qDebug()<<objMap["userid"].toInt();
                            userInfo.nUserId = objMap["userid"].toInt();
                        }
                    }
                }

                userList->append(userInfo);
            }
        }
    }
}
