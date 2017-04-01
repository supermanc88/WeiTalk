#include "singlechat.h"
#include "ui_singlechat.h"
#include "QXmppMessage.h"
#include <QMessageBox>

#include <QClipboard>
#include <QPixmap>

typedef int (__stdcall *FnStartScreenCaptureW)(const wchar_t* szDefaultSavePath, void* pCallBack, UINT_PTR hWndNotice, UINT_PTR noticeMsg, UINT_PTR hwndHideWhenCapture, int autoCapture, int x, int y, int width, int height);
extern FnStartScreenCaptureW gl_StartScreenCapture; //截图全局函数

SingleChat * singleChatPointer;

SingleChat::SingleChat(const QString& bareJid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleChat)
{
    ui->setupUi(this);

    singleChatPointer = this;

    ui->nickname->setText(bareJid);
    this->bareJid = bareJid;

    setAttribute(Qt::WA_QuitOnClose, false);

    setWindowFlags(Qt::FramelessWindowHint);

    connect(ui->myLabel, SIGNAL(clicked()), this, SLOT(ShowMinimize()));
    connect(ui->myLabel_2, SIGNAL(clicked()), this, SLOT(CloseCurrentWindow()));

    //connect btn to send message
    connect(ui->sendMessageBtn, SIGNAL(clicked(bool)), this, SLOT(SingleSendMessage()));


    //connect client to receive some message
//    connect(this->client, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(messageReceived(QXmppMessage)));

    connect(ui->captureBtn, SIGNAL(clicked(bool)), this, SLOT(onCapture()));
    connect(this, SIGNAL(captureFinished(int)), this, SLOT(OnCaptureFinish(int)));
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
    QString sendString = this->ui->textArea->text();
    client->sendMessage(this->bareJid,sendString);
    this->ui->textArea->setText("");

    ui->textBrowser->insertPlainText(this->bareJid + ": " + "\n");
    ui->textBrowser->insertPlainText("  " + sendString + "\n");
}

void SingleChat::messageReceived(const QXmppMessage &message)
{
    qDebug()<<message.body();
}

void SingleChat::setChatContent(QString message)
{
    ui->textBrowser->insertPlainText(message);
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

void CaptureNotice(int nType, int x, int y, int width, int height, const char *szInfo)
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
