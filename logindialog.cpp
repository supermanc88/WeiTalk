#include "logindialog.h"
#include <QMouseEvent>
#include <QIcon>

#include <QAction>

//#include "frienddialog.h"

#include "QXmppRosterManager.h"
#include "QXmppConfiguration.h"
#include <QApplication>
#include <QDesktopWidget>

#include <QPoint>

#include <QPropertyAnimation>

/*
 * setAttribute(Qt::WA_QuitOnClose, true);
 * setAttribute(Qt::WA_QuitOnClose, false);
 * 在主窗口设置为true，别的窗口设置为false
 * 当关闭主窗口时，所有的窗口也将被关闭
 */
QString LoginUserName;   //全局的登录用户名

LoginDialog::LoginDialog(QXmppClient *xmppClient, QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);   //不显示标题栏 & 始终保持顶端
    setupUi(this);

    setAttribute(Qt::WA_QuitOnClose, false);

    m_moving = false;

    //初始化托盘及托盘菜单
    m_trayIcon = new QSystemTrayIcon;
    initSysTrayContextMenu();

    //LoginDialog动画开启
    QPropertyAnimation *init_animation = new QPropertyAnimation(this, "windowOpacity");
    init_animation->setDuration(500);
    init_animation->setStartValue(0);
    init_animation->setEndValue(1);
    init_animation->start();

    //初始化登录画面
    initLoginAnimation();

    connect(this->myLabel, SIGNAL(clicked()), this, SLOT(ShowMinimize()));
    connect(this->myLabel_2, SIGNAL(clicked()), this, SLOT(CloseWindow()));

    //创建QXmppClient
    this->m_xmppClient = xmppClient;


    connect(this->m_xmppClient, SIGNAL(disconnected()), this, SLOT(showSignInPageAfterUserDisconnection()));
    connect(this->m_xmppClient, SIGNAL(connected()), this, SIGNAL(loginSucess()));

//    connect(&this->m_xmppClient->rosterManager(), SIGNAL(rosterReceived()), this, SLOT(rosterReceived()));

    //登录成功后，隐藏此登录界面
    connect(this, SIGNAL(loginSucess()), this, SLOT(hide()));
}

void LoginDialog::mousePressEvent(QMouseEvent *event)
{
    m_moving = true;
    //记录下当前鼠标相对于窗口的位置
    m_movePosition = event->globalPos() - this->pos();
    return QDialog::mousePressEvent(event);
}

void LoginDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_moving = false;
    return QDialog::mouseReleaseEvent(event);
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() && Qt::LeftButton &&
            (event->globalPos()-m_movePosition).manhattanLength() > QApplication::startDragDistance())
    {
        this->move(event->globalPos() - m_movePosition);
        return QDialog::mouseMoveEvent(event);
    }
}

void LoginDialog::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:

        break;

    case QSystemTrayIcon::DoubleClick:
        this->show();
        break;

    default:
        break;
    }
}

void LoginDialog::initSysTrayContextMenu()
{
    m_sysTrayMenu = new QMenu;
    //菜单栏里的动作

    QAction *imaway, *imbusy, *imcallme, *imhidden, *imoffline, *imonline, *imsilent;

    imaway = new QAction(tr("离开"), this);
    imbusy = new QAction(tr("忙碌"), this);
    imcallme = new QAction(tr("联系我吧"), this);
    imhidden = new QAction(tr("隐身"), this);
    imoffline = new QAction(tr("离线"), this);
    imonline = new QAction(tr("我在线上"), this);
    imsilent = new QAction(tr("请勿打扰"), this);

    imaway->setIcon(QIcon(":/images/imaway.png"));
    imbusy->setIcon(QIcon(":/images/imbusy.png"));
    imcallme->setIcon(QIcon(":/images/imcallme.png"));
    imhidden->setIcon(QIcon(":/images/imhidden.png"));
    imoffline->setIcon(QIcon(":/images/imoffline.png"));
    imonline->setIcon(QIcon(":/images/imonline.png"));
    imsilent->setIcon(QIcon(":/images/imsilent.png"));

    m_sysTrayMenu->addAction(imaway);
    m_sysTrayMenu->addAction(imbusy);
    m_sysTrayMenu->addAction(imcallme);
    m_sysTrayMenu->addAction(imhidden);
    m_sysTrayMenu->addAction(imoffline);
    m_sysTrayMenu->addAction(imonline);
    m_sysTrayMenu->addAction(imsilent);

}

void LoginDialog::initLoginAnimation()
{
    //pic_animation向下移动
    QPropertyAnimation *pic_animation = new QPropertyAnimation(this->pictureLabel, "pos");
    pic_startX = this->pictureLabel->pos().x();
    pic_startY = this->pictureLabel->pos().y();
    int endX = pic_startX;
    int endY = pic_startY + 80;
    pic_animation->setDuration(1000);
    pic_animation->setStartValue(QPoint(pic_startX, pic_startY));
    pic_animation->setEndValue(QPoint(endX, endY));


    //loginPushButton向上移动
//    QPropertyAnimation *loginBtn_animation = new QPropertyAnimation(this->loginPushButton, "pos");
//    startX = this->loginPushButton->pos().x();
//    startY = this->loginPushButton->pos().y();
//    endX = startX;
//    endY = startY - 30;
//    loginBtn_animation->setDuration(500);
//    loginBtn_animation->setStartValue(QPoint(startX, startY));
//    loginBtn_animation->setEndValue(QPoint(endX, endY));

    m_loginAnimationGroup = new QSequentialAnimationGroup;
    m_loginAnimationGroup->addAnimation(pic_animation);
//    m_loginAnimationGroup->addAnimation(loginBtn_animation);
}

void LoginDialog::on_loginPushButton_clicked()
{
    //判断登录按钮的状态是登录还是取消
    if(this->loginPushButton->text() == "登录")
    {
        //显示登录时界面
        showSignInPage();

        //获取账号密码开始登录
        signIn();

        //这里到最后是需要响应的，先只做效果
        //不用等动画完成再显示登录了，只要client接收到登录成功信号即可
//        connect(this->m_loginAnimationGroup, SIGNAL(finished()), this->m_xmppClient, SIGNAL(connected()));
//        loginSucess();
    }
    else
    {
        //显示取消界面
        showSignInPageAfterUserDisconnection();
        //取消连接服务器
        cancelSignIn();
    }


}

void LoginDialog::signIn()
{
    QString userName = this->UserNameLineEdit->text();
    QString passwd = this->PasswordLineEdit->text();

    LoginUserName = userName;

    //userName 需要进行拼接
    QString bareJid = userName + QString("@im.weitainet.com/gloox");

    this->m_xmppClient->configuration().setJid(bareJid);
    this->m_xmppClient->configuration().setPassword(passwd);

    startConnection();//连接
}

void LoginDialog::cancelSignIn()
{
    this->m_xmppClient->disconnectFromServer();
}

void LoginDialog::showSignInPage()
{
    m_loginAnimationGroup->start();
    //除了登录按钮和图片laber全部隐藏
    this->UserNameLineEdit->setVisible(false);
    this->PasswordLineEdit->setVisible(false);
    this->autoLoginCheckBox->setVisible(false);
    this->remenberCheckBox->setVisible(false);
    this->getPasswordLabel->setVisible(false);
    this->registerLabel->setVisible(false);

    //登录按钮改成取消
    this->loginPushButton->setText(tr("取消"));
}

void LoginDialog::showSignInPageAfterUserDisconnection()
{
    //停止动画,并且把图片移动回原来位置
    m_loginAnimationGroup->stop();
    this->pictureLabel->move(QPoint(pic_startX, pic_startY));

    //显示隐藏的控件
    this->UserNameLineEdit->setVisible(true);
    this->PasswordLineEdit->setVisible(true);
    this->autoLoginCheckBox->setVisible(true);
    this->remenberCheckBox->setVisible(true);
    this->getPasswordLabel->setVisible(true);
    this->registerLabel->setVisible(true);

    //取消按钮改成登录
    this->loginPushButton->setText(tr("登录"));
}

//void LoginDialog::rosterReceived()
//{
//    qDebug("example_2_rosterHandling:: Roster received");
//    foreach (const QString &bareJid, this->m_xmppClient->rosterManager().getRosterBareJids()) {
//        QString name = this->m_xmppClient->rosterManager().getRosterEntry(bareJid).name();
//        if(name.isEmpty())
//            name = "-";
//        qDebug("example_2_rosterHandling:: Roster received: %s [%s]", bareJid.toStdString().c_str(), qPrintable(name));
//    }

//}


//LoginDialog淡化关闭动画
void LoginDialog::CloseWindow()
{
    QPropertyAnimation *close_animation = new QPropertyAnimation(this, "windowOpacity");
    close_animation->setDuration(500);
    close_animation->setStartValue(1);
    close_animation->setEndValue(0);
    close_animation->start();

    //关闭应用程序
    connect(close_animation, SIGNAL(finished()), qApp, SLOT(quit()));
}

//最小化到托盘
void LoginDialog::ShowMinimize()
{
    this->hide();

    QIcon icon = QIcon(":/images/avatar.png");

    m_trayIcon->setIcon(icon);

    m_trayIcon->setToolTip(tr("显示托盘"));

    //给QSystemTrayIcon添加槽函数
    connect(m_trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

    m_trayIcon->setContextMenu(m_sysTrayMenu);
    m_trayIcon->show();
}

void LoginDialog::startConnection()
{
    this->m_xmppClient->connectToServer(this->m_xmppClient->configuration());
}
