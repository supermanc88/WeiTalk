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
    QString serverName = "upload.weitainet.com";
    QString strHTTPBoundary = "---------------------------7b4a6d158c9";//定义边界值
    QString defObjectName = "/upload/upload/?typeid=13&iswkt=1&framename=${framename}";//保存的地址

    QFile picFile(path);
    qint64 picFileSize = picFile.size();

    QString strPreFileData = MakePreFileData(strHTTPBoundary, path, 1);
    QString strPostFileData = MakePostFileData(strHTTPBoundary);

    qint64 dwTotalRequestLength = strPreFileData.length() + strPostFileData.length() + picFileSize;//计算整个包的总长度

    QNetworkAccessManager * manager = new QNetworkAccessManager;
    QNetworkRequest * request = new QNetworkRequest;
    request->setUrl(serverName + defObjectName);
    request->setHeader(QNetworkRequest::ContentTypeHeader, QString("multipart/form-data; boundary=%1\r\n").arg(strHTTPBoundary));
    request->setHeader(QNetworkRequest::ContentLengthHeader, dwTotalRequestLength);

    QByteArray upLoadData = QByteArray();

    upLoadData.append(strPreFileData);
    upLoadData.append(picFile.readAll());
    upLoadData.append(strPostFileData);

    QNetworkReply * reply = manager->post(request, upLoadData);



}

QString WeiTalkServerAPI::MakePreFileData(QString &strBoundary, QString &strFileName, int iRecordID)
{
    QString strFormat;
    QString strData;

    strFormat += "--%1";
    strFormat += "\r\n";
    strFormat += "Content-Disposition: form-data; name=\"para1\"";//传给网络上的参数，根据网站抓包查看到底是需要哪些
    strFormat += "\r\n\r\n";
    strFormat += "my name is xiaoxiong";
    strFormat += "\r\n";

    strFormat += "--%2";
    strFormat += "\r\n";
    strFormat += "Content-Disposition: form-data; name=\"para2\"";
    strFormat += "\r\n\r\n";
    strFormat += "微太网络科技有限公司";
    strFormat += "\r\n";

    strFormat += "--%3";
    strFormat += "\r\n";
    strFormat += "Content-Disposition: form-data; name=\"trackdata\"; filename=\"%4\"";//文件地址信息
    strFormat += "\r\n";
    strFormat += "Content-Type: image/bmp";

    strFormat += "\r\n\r\n";
//    strData.Format(strFormat,  strBoundary, strBoundary, strBoundary, strFileName);//
    strData = QString(strFormat).arg(strBoundary).arg(strBoundary).arg(strBoundary).arg(strFileName);
    return strData;
}

QString WeiTalkServerAPI::MakePostFileData(QString &strBoundary)
{
    QString strFormat;
    QString strData;

    strFormat = "\r\n";
    strFormat += "--%1";
    strFormat += "\r\n";
    strFormat += "Content-Disposition: form-data; name=\"submitted\"";
    strFormat += "\r\n\r\n";
    strFormat += "hello";
    strFormat += "\r\n";
    strFormat += "--%2--";
    strFormat += "\r\n";

//    strData.Format(strFormat, strBoundary, strBoundary);

    strData = QString(strFormat).arg(strBoundary).arg(strBoundary);

    return strData;
}

QString WeiTalkServerAPI::MakeRequestHeaders(QString &strBoundary)
{
    QString strFormat;
    CStringA strData;

    strFormat = "Content-Type: multipart/form-data; boundary=%1\r\n";//二进制文件传送Content-Type类型为: multipart/form-data

//    strData.Format(strFormat, strBoundary);
    strData = QString(strFormat).arg(strBoundary);
    return strData;
}
