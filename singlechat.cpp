#include "singlechat.h"
#include "ui_singlechat.h"
#include "QXmppMessage.h"
#include <QMessageBox>

#include <QClipboard>
#include <QPixmap>

#include <QFile>

#include <QCryptographicHash>
#include <QDomDocument>
#include <QDomElement>
#include <QDir>

typedef int (__stdcall *FnStartScreenCaptureW)(const wchar_t* szDefaultSavePath, void* pCallBack, UINT_PTR hWndNotice, UINT_PTR noticeMsg, UINT_PTR hwndHideWhenCapture, int autoCapture, int x, int y, int width, int height);
extern FnStartScreenCaptureW gl_StartScreenCapture; //截图全局函数

SingleChat * singleChatPointer;
extern QString newPicPath;  //最初的定义在frienddialog
extern QString currentChattingJID; //最初的定义在frienddialog
extern QString LoginUserName;

SingleChat::SingleChat(const QString& bareJid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleChat)
{
    ui->setupUi(this);

    singleChatPointer = this;

    ui->nickname->setText(bareJid);
    this->bareJid = bareJid;

    setAttribute(Qt::WA_QuitOnClose, false);

//    setWindowFlags(Qt::FramelessWindowHint);

    SendThreadA = new SendPicThread();

    /*********************emotion 初始化 start***********/
    emotion = new MyEmotion;
    emotion->initEmotion();
    /*********************emotion 初始化 end*************/

    connect(ui->myLabel, SIGNAL(clicked()), this, SLOT(ShowMinimize()));
    connect(ui->myLabel_2, SIGNAL(clicked()), this, SLOT(CloseCurrentWindow()));

    //connect btn to send message
    connect(ui->sendMessageBtn, SIGNAL(clicked(bool)), this, SLOT(SingleSendMessage()));


    //connect client to receive some message
//    connect(this->client, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(messageReceived(QXmppMessage)));

    connect(ui->captureBtn, SIGNAL(clicked(bool)), this, SLOT(onCapture()));
    connect(this, SIGNAL(captureFinished(int)), this, SLOT(OnCaptureFinish(int)));
    connect(this, SIGNAL(insertCapture()), this, SLOT(InsertCapture()));

    connect(SendThreadA, SIGNAL(finishedUpLoadPic()), this, SLOT(sendSinglePic()));

    //关联表情点击
    connect(this->ui->emotionBtn, SIGNAL(clicked(bool)), this, SLOT(showEmotion()));

    connect(this->emotion, SIGNAL(cellClicked(int,int)), this, SLOT(insertEmotion(int, int)));
}

SingleChat::~SingleChat()
{
    delete ui;
}

void SingleChat::ShowMinimize()
{
    this->showMinimized();
}

void SingleChat::CloseCurrentWindow()
{
    emit closeSingleChat(this->bareJid);
    this->close();
}

void SingleChat::SingleSendMessage()
{
    QString sendString = this->ui->textEdit->toHtml();

    //判断发送的信息中是否包含图片
    QDomDocument doc;
    doc.setContent(sendString);
    QDomElement docElement = doc.documentElement();

    QDomNodeList nodeList = docElement.elementsByTagName("img");

    if(nodeList.count() == 0)
    {

    }
    else
    {
        SendThreadA->start();
        for(int i=0; i<nodeList.count(); i++)
        {
            QDomNode node = nodeList.at(i);
            //获取img节点的父节点 p节点
            QDomNode parentNode = node.parentNode();

            //删除当前的p节点
            parentNode.parentNode().removeChild(parentNode);

            qDebug()<<"dd删除节点";
        }
    }
    sendString = doc.toString();
    qDebug()<<sendString;

    client->sendMessage(this->bareJid,sendString);
    this->ui->textEdit->clear();

    this->ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->insertPlainText("\r\n" + LoginUserName + ": " + "\r\n");
    ui->textBrowser->insertHtml("  " + sendString + "\r\n");
    this->ui->textBrowser->textCursor().movePosition(QTextCursor::End);
    this->ui->textBrowser->moveCursor(QTextCursor::End);
}

void SingleChat::messageReceived(const QXmppMessage &message)
{
    qDebug()<<message.body();
}

void SingleChat::setChatText(QString message)
{
    this->ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->insertPlainText("\r\n");
    this->ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->insertHtml(message);
    this->ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->insertPlainText("\r\n");
    this->ui->textBrowser->textCursor().movePosition(QTextCursor::End);
    this->ui->textBrowser->moveCursor(QTextCursor::End);
}

void SingleChat::setChatContent(QString message)
{
    this->ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->insertHtml(message);
    this->ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->insertPlainText("\r\n");
    this->ui->textBrowser->textCursor().movePosition(QTextCursor::End);
    this->ui->textBrowser->moveCursor(QTextCursor::End);
}

void SingleChat::sendSinglePic()
{
    QString sentHtml = QString("<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.0//EN' 'http://www.w3.org/TR/REC-html40/strict.dtd'>\n<html>\n <head>\n  <meta content=\"1\" name=\"qrichtext\"/>\n  <style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style>\n </head>\n <body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n  <p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\n   <img src=\"http://upload.weitainet.com%1\"/>\n  </p>\n  <p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\n   <br/>\n  </p>\n </body>\n</html>\n").arg(newPicPath);

    QString userJID = bareJid;

    currentChattingJID = userJID;

    client->sendMessage(userJID, sentHtml);

    this->ui->textBrowser->insertPlainText("\r\n" + LoginUserName + ": " + "\r\n");
    this->ui->textBrowser->insertHtml(sentHtml + "\r\n");

}

void SingleChat::showEmotion()
{
    if(emotion->isHidden())
    {
        emotion->show();
    }
    else
    {
        emotion->hide();
    }

}

void SingleChat::onCapture()
{
    const wchar_t* szSavePath = (QString("c:\\测试一下\\test.jpg").toStdWString().c_str());
    gl_StartScreenCapture(szSavePath, CaptureNotice, 0, 0, 0, 0, 0, 0, 0, 0);

}

void SingleChat::OnCaptureFinish(int type)
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

void SingleChat::InsertCapture()
{
    ui->textEdit->insertHtml(uploadPicPath);
    ui->textEdit->moveCursor(QTextCursor::End);
}

void SingleChat::insertEmotion(int row, int column)
{
    int emotionId = row * 6 + column;
    qDebug()<<"emotionId: "<<emotionId;
    this->ui->textBrowser->moveCursor(QTextCursor::End);

    this->ui->textBrowser->insertPlainText(QString("[\\e]%1").arg(emotionId));
    this->ui->textBrowser->moveCursor(QTextCursor::End);
}

void CaptureNotice(int nType, int x, int y, int width, int height, const char *szInfo)
{
    if (nType == 1)	//表示截图完成
    {
        qDebug()<<QString(szInfo);
        qDebug()<<"width:"<<width<<"heigth:"<<height;
        QClipboard * board = QApplication::clipboard();

        QPixmap pixmap = board->pixmap();

        QDir dir(QCoreApplication::applicationDirPath());
        if(!dir.exists("capture"))
        {
            dir.mkdir("capture");
        }

        //文件的名字是文件的md5值
        pixmap.save(QCoreApplication::applicationDirPath()+"/capture/"+"90[~VYQFS][Z2}ADFASDFADSFCXVZZ.jpg");

        QString picPath = QCoreApplication::applicationDirPath()+"/capture/"+"90[~VYQFS][Z2}ADFASDFADSFCXVZZ.jpg";
        QFile theFile(picPath);

        theFile.open(QIODevice::ReadOnly);
        QByteArray ba = QCryptographicHash::hash(theFile.readAll(),QCryptographicHash::Md5);
        theFile.close();

        newPicPath = QCoreApplication::applicationDirPath()+"/capture/"+QString(ba.toHex())+".jpg";
        qDebug()<<newPicPath;
        bool isRename = QFile::rename(picPath, newPicPath);
        Q_ASSERT(isRename);

        QString uploadPicPath = QString("<img src=\"%1\"/>").arg(newPicPath);

        singleChatPointer->uploadPicPath = uploadPicPath;
        emit singleChatPointer->insertCapture();
    }
    else if(nType == 2)	//表示取消截图
    {

    }
    else		//保存截图
    {

    }
//    ::PostMessage(gl_hWnd, WM_USER + 1111, 1, nType);
    emit singleChatPointer->captureFinished(nType);
}

QString SingleChat::getContentOfSend() const
{
//    contentOfSend = this->ui->sendTextArea->
//    this->contentOfSend = ui->textArea->text();
    return contentOfSend;
}

void SingleChat::setClient(QXmppClient *value)
{
    client = value;
}
