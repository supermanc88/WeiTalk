#include "wechat.h"
#include "ui_wechat.h"

#include <QVBoxLayout>
#include <groupmembermodel.h>

#include <QDebug>

#include <QMessageBox>
#include <QClipboard>
#include <QPixmap>

extern QXmppClient * globalClient;
extern QString LoginUserName;

WeChat * weChatPointer;

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

    API.SelectGroupUserList(this->groupId, groupMemberList, this->memberCount);

    qDebug()<<this->groupId;

    //connect sendmessagebtn to funtion sendmessage
    connect(this->ui->sendMessageBtn, SIGNAL(clicked(bool)), this, SLOT(sendMessage()));

    connect(this->ui->captureBtn, SIGNAL(clicked(bool)), this, SLOT(onCapture()));
    connect(this, SIGNAL(captureFinished(int)), this, SLOT(OnCaptureFinish(int)));

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

void WeChat::sendMessage()
{
    QString sendText = this->ui->lineEdit->text();

    QString groupJID = getGroupJID();

    client->sendMessage(groupJID, sendText);

    this->ui->lineEdit->setText("");

    this->ui->textBrowser->insertPlainText(LoginUserName + ": " + "\n");
    this->ui->textBrowser->insertPlainText("    " + sendText + "\n");

}

void WeChat::setChatContent(QString message)
{
    this->ui->textBrowser->insertPlainText(message);
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

void CaptureNoticeWeChat(int nType, int x, int y, int width, int height, const char *szInfo)
{
    if (nType == 1)	//表示截图完成
    {
        qDebug()<<QString(szInfo);
        qDebug()<<"width:"<<width<<"heigth:"<<height;
        QClipboard * board = QApplication::clipboard();

        QPixmap pixmap = board->pixmap();
        pixmap.save("a.png");

        QString picPath = QCoreApplication::applicationDirPath()+"/a.png";

        QString uploadPicPath = QString("<img src=\"%1\"/>").arg(picPath);

//        thisPointer->ui->label->setPixmap(pixmap.scaled(200,200));

//        thisPointer->ui->textBrowser->insertHtml(uploadPicPath);
//        thisPointer->ui->label->setPixmap(board->pixmap());
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

