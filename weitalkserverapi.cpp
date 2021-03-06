#include "weitalkserverapi.h"
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariantMap>
#include <QDebug>
#include <QFile>


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

void WeiTalkServerAPI::SelectGroupText(const QString groupName, QList<group_info_t> *groupList)
{
    QString url = "http://api.weitainet.com/router/rest?method=weitai.im.search&groupname=%1";

    QByteArray responseData = getHtml(url.arg(groupName));

    QJsonDocument d = QJsonDocument::fromJson(responseData);

    QJsonObject obj = d.object();

    if(obj.contains("rows"))
    {
        QJsonArray rows = obj["rows"].toArray();

        if(rows.size() == 0)
        {
            return ;
        }

        for(int i = 0; i < rows.size(); i++)
        {
            group_info_t groupInfo;

            if(rows[i].isObject())
            {
                QJsonObject subObj = rows[i].toObject();
                QVariantMap objMap = subObj.toVariantMap();

                groupInfo.acGroupName = objMap["groupname"].toString();
                groupInfo.acGroupOwn = objMap["creater"].toString();
                groupInfo.acDescription = objMap["fingermemo"].toString();
                groupInfo.nGroupCount = objMap["num"].toInt();
                groupInfo.nGroupId = objMap["groupid"].toInt();
            }

            groupList->append(groupInfo);
        }

    }
}

void WeiTalkServerAPI::SelectGroupList(const QString username, QList<group_info_t> *groupList)
{
    QString url = "http://api.weitainet.com/router/rest?method=weitai.im.search&username=%1";

    QByteArray responseData = getHtml(url.arg(username));

    QJsonDocument d = QJsonDocument::fromJson(responseData);

    QJsonObject obj = d.object();

    if(obj.contains("rows"))
    {
        QJsonArray rows = obj["rows"].toArray();
        if(rows.size() == 0)
        {
            return;
        }

        for(int i = 0; i < rows.size(); i++)
        {
            group_info_t groupInfo;

            if(rows[i].isObject())
            {
                QJsonObject subObj = rows[i].toObject();
                QVariantMap objMap = subObj.toVariantMap();

                groupInfo.acGroupName = objMap["groupname"].toString();
                groupInfo.acGroupOwn = objMap["creater"].toString();
                groupInfo.acDescription = objMap["fingermemo"].toString();
                groupInfo.nGroupCount = objMap["num"].toInt();
                groupInfo.nGroupId = objMap["groupid"].toInt();
            }

            groupList->append(groupInfo);
        }
    }
}

void WeiTalkServerAPI::SelectGroupUserList(unsigned int groupId, QList<group_member_t> *groupMemberList, int& memberCount)
{
    QString url = "http://api.weitainet.com/router/rest?method=weitai.im.getgrouplist&groupid=%1";

    QByteArray responseData = getHtml(url.arg(groupId));

    QJsonDocument d = QJsonDocument::fromJson(responseData);

    QJsonObject obj = d.object();

    if(obj.contains("model"))
    {
        QJsonArray models = obj["model"].toArray();

        memberCount = models.size();

        for(int i=0; i<models.size(); i++)
        {
            group_member_t member;
            if(models[i].isObject())
            {
                QJsonObject subObj = models[i].toObject();
                QVariantMap objMap = subObj.toVariantMap();

                member.authority = objMap["authority"].toInt();
                member.groupid = objMap["groupid"].toInt();
                member.nickname = objMap["nickname"].toString();
                member.remark = objMap["remark"].toString();
                member.userid = objMap["userid"].toInt();
                member.username = objMap["username"].toString();
            }
            groupMemberList->append(member);
        }
    }

}

BOOL WeiTalkServerAPI::OnUploadPic(QString &path)
{
    QString serverName = "http://upload.weitainet.com";
    QString defObjectName = "/upload/upload/?typeid=13&iswkt=1&framename=${framename}";//保存的地址

    QUrl url(serverName+defObjectName);

    QFile picFile(path);
    qDebug()<<picFile.fileName();

    QString picName = picFile.fileName().section("/", -1, -1);

    QNetworkAccessManager * manager = new QNetworkAccessManager;
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=----WebKitFormBoundaryaKEu6bf7G9MqHFNr");

    QByteArray upLoadData = QByteArray();

    upLoadData.append(QString("------WebKitFormBoundaryaKEu6bf7G9MqHFNr\r\nContent-Disposition: form-data; name=\"file\"; filename=\"%1\"\r\n\r\nContent-Type: image/jpeg\r\n\r\n").arg(picName));

    if(!picFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    upLoadData.append(picFile.readAll());

    upLoadData.append("\r\n------WebKitFormBoundaryaKEu6bf7G9MqHFNr\r\nContent-Disposition: form-data; name=\"submit\"\r\n\r\ntijiao\r\n------WebKitFormBoundaryaKEu6bf7G9MqHFNr--\r\n");

    QNetworkReply * reply = manager->post(request, upLoadData);

    QByteArray responseData;
    QEventLoop eventLoop;

    connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    responseData = reply->readAll();
    //返回json格式"{"filepath":"/uploadfile/13/jpg/2017/04/01/3b2d80ef1059b4b6f08cf0613927c238.jpg","framename":"${framename}","typeid":13,"name":"D:\\WeiTaiTalk\\test11\\C2C\\003b2d80ef1059b4b6f08cf0613927c238.jpg","extname":".jpg","success":true}"	ATL::CStringT<char,StrTraitMFC_DLL<char,ATL::ChTraitsCRT<char> > >

    QJsonDocument d = QJsonDocument::fromJson(responseData);
    QJsonObject obj = d.object();

    if(obj.contains("filepath"))
    {
        path = obj["filepath"].toString();
        qDebug()<<path;
        return true;
    }
    else
    {
        false;
    }

}

