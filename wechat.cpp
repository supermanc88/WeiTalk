#include "wechat.h"
#include "ui_wechat.h"

#include <QVBoxLayout>
#include <groupmembermodel.h>

#include <QDebug>

#include <QMessageBox>
#include <QClipboard>
#include <QPixmap>

#include <QDomDocument>
#include <QDomElement>

#include "sendpicthread.h"

extern QXmppClient * globalClient;
extern QString LoginUserName;

WeChat * weChatPointer;

extern QString newPicPath;  //最初的定义在frienddialog
extern QString currentChattingJID; //最初的定义在frienddialog

typedef int (__stdcall *FnStartScreenCaptureW)(const wchar_t* szDefaultSavePath, void* pCallBack, UINT_PTR hWndNotice, UINT_PTR noticeMsg, UINT_PTR hwndHideWhenCapture, int autoCapture, int x, int y, int width, int height);
extern FnStartScreenCaptureW gl_StartScreenCapture; //截图全局函数


WeChat::WeChat(int groupId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeChat)
{
    ui->setupUi(this);

    weChatPointer = this;

    this->groupId = groupId;

    memberCount = 0;
    g_layout = new QVBoxLayout;
    g_layout->setContentsMargins(1,0,7,0);
    g_layout->setSpacing(2);

    groupMemberList = new QList<group_member_t>;
    groupMemberMap = new QMap<QString, GroupMemberModel*>;

    client = globalClient;

    SendThreadA = new SendPicThread();

    API.SelectGroupUserList(this->groupId, groupMemberList, this->memberCount);

    qDebug()<<this->groupId;

    //connect sendmessagebtn to funtion sendmessage
    connect(this->ui->sendMessageBtn, SIGNAL(clicked(bool)), this, SLOT(sendMessage()));

    connect(this->ui->captureBtn, SIGNAL(clicked(bool)), this, SLOT(onCapture()));
    connect(this, SIGNAL(captureFinished(int)), this, SLOT(OnCaptureFinish(int)));
    connect(this, SIGNAL(insertCapture()), this, SLOT(InsertCapture()));

    connect(SendThreadA, SIGNAL(finishedUpLoadPic()), this, SLOT(sendSinglePic()));

}

WeChat::~WeChat()
{
    delete ui;
}

void WeChat::initGroupMemberList()
{
    QList<group_member_t>::iterator iter;

    for(iter = groupMemberList->begin(); iter != groupMemberList->end(); iter++)
    {
        GroupMemberModel * model = new GroupMemberModel;
        QString c_nickName = (*iter).nickname;
        QString c_userName = (*iter).username;

        groupMemberMap->insert(c_userName, model);

        //如果有昵称的话设置的是昵称，没有就设置用户名
        if(!c_nickName.isEmpty())
        {
            model->setNickName(c_nickName);
        }
        else
        {
            model->setNickName(c_userName);
        }

        int count = g_layout->count();
        if(count > 0)
        {
            g_layout->removeItem(g_layout->itemAt(count - 1));
        }
        g_layout->addWidget(model);
        g_layout->addStretch();
    }

    this->ui->scrollAreaWidgetContents->setLayout(g_layout);
}

QString WeChat::getGroupName() const
{
    return groupName;
}

void WeChat::setGroupName(const QString &value)
{
    groupName = value;

    this->ui->groupNameLabel->setText(groupName);
}

QMap<QString, GroupMemberModel *> *WeChat::getGroupMemberMap() const
{
    return groupMemberMap;
}

QString WeChat::getGroupJID()
{
    QString strGid = QString("%1").arg(this->groupId);

    QString groupJID = strGid + "@conference.im.weitainet.com";

    qDebug()<<"room jid:"<<groupJID;
    return groupJID;
}

/**
 * 这里需要把图片和文字分开发送
 */
void WeChat::sendMessage()
{
    QString sendText = this->ui->textEdit->toHtml();

    //判断发送的信息中是否包含图片
    QDomDocument doc;
    doc.setContent(sendText);
    QDomElement docElement = doc.documentElement();

    QDomNodeList nodeList = docElement.elementsByTagName("img");

    if(nodeList.count() == 0)
    {

    }
    else
    {
        SendThreadA->start();
//        for(int i=0; i<nodeList.count(); i++)
//        {
//            QDomNode node = nodeList.at(i);
//            qDebug()<< node.toElement().attribute("src");

//            //如果存在src 就替换成上传的地址
//            bool isupload = WTAPI.OnUploadPic(newPicPath);
//            Q_ASSERT(isupload);

//            node.toElement().setAttribute("src", "http://upload.weitainet.com"+newPicPath);
//        }
    }


//    sendText = doc.toString();

//    sendText = this->ui->textEdit->toPlainText().toHtmlEscaped();

    QString groupJID = getGroupJID();

    currentChattingJID = groupJID;

    client->sendMessage(groupJID, sendText);

    this->ui->textEdit->clear();

    this->ui->textBrowser->insertPlainText("\r\n" + LoginUserName + ": " + "\r\n");
    this->ui->textBrowser->insertHtml("    " + sendText + "\r\n");

}

void WeChat::setChatContent(QString message)
{
    this->ui->textBrowser->insertHtml(message);
}

void WeChat::sendSinglePic()
{
    QString sentHtml = QString("<!DOCTYPE HTML PUBLIC \"-/W3C/DTD HTML 4.0/EN\" \"http:/www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\"http://upload.weitainet.com%1\" /></p></body></html>").arg(newPicPath);

    QString groupJID = getGroupJID();

    currentChattingJID = groupJID;

    client->sendMessage(groupJID, sentHtml);

    this->ui->textBrowser->insertPlainText("\r\n" + LoginUserName + ": " + "\r\n");
    this->ui->textBrowser->insertHtml(sentHtml + "\r\n");
}

void WeChat::onCapture()
{
    const wchar_t* szSavePath = (QString("c:\\测试一下\\test.jpg").toStdWString().c_str());
    gl_StartScreenCapture(szSavePath, CaptureNoticeWeChat, 0, 0, 0, 0, 0, 0, 0, 0);

}

void WeChat::OnCaptureFinish(int type)
{
    switch (type) {
    case 1:
        QMessageBox::information(this, "title", "截图完毕",QMessageBox::Ok);
        break;
    case 2:
        QMessageBox::information(this, "title", "取消截图",QMessageBox::Ok);
        break;
    case 3:
        QMessageBox::information(this, "title", "保存截图",QMessageBox::Ok);
        break;
    default:
        break;
    }
}

void WeChat::InsertCapture()
{
    ui->textEdit->insertHtml(uploadPicPath);
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->insertPlainText("\r\n");      //添加一个换行符
}

void CaptureNoticeWeChat(int nType, int x, int y, int width, int height, const char *szInfo)
{
    if (nType == 1)	//表示截图完成
    {
        qDebug()<<QString(szInfo);
        qDebug()<<"width:"<<width<<"heigth:"<<height;
        QClipboard * board = QApplication::clipboard();

        QPixmap pixmap = board->pixmap();

        //文件的名字是文件的md5值
        pixmap.save("90[~VYQFS][Z2}ADFASDFADSFCXVZZ.jpg");

        QString picPath = QCoreApplication::applicationDirPath()+"/90[~VYQFS][Z2}ADFASDFADSFCXVZZ.jpg";
        QFile theFile(picPath);

        theFile.open(QIODevice::ReadOnly);
        QByteArray ba = QCryptographicHash::hash(theFile.readAll(),QCryptographicHash::Md5);
        theFile.close();

        newPicPath = QCoreApplication::applicationDirPath()+"/"+QString(ba.toHex())+".jpg";
        qDebug()<<newPicPath;
        bool isRename = QFile::rename(picPath, newPicPath);
        Q_ASSERT(isRename);

        QString uploadPicPath = QString("<img src=\"%1\"/>").arg(newPicPath);

        weChatPointer->uploadPicPath = uploadPicPath;

        emit weChatPointer->insertCapture();

    }
    else if(nType == 2)	//表示取消截图
    {

    }
    else		//保存截图
    {

    }
//    ::PostMessage(gl_hWnd, WM_USER + 1111, 1, nType);
    emit weChatPointer->captureFinished(nType);
}

