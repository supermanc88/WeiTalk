#include "frienddialog.h"
#include <QMouseEvent>
#include <QtDebug>

#include <QVBoxLayout>
#include <QScrollArea>
#include <QWidget>
#include <QPushButton>
#include "friendlistarea.h"
#include "frienditem.h"
#include "imtoolbox.h"
//#include "imtoolitem.h"
#include <QMessageBox>
#include <QDesktopWidget>

#include "QXmppRosterManager.h"
#include <QInputDialog>

#include "QXmppRosterIq.h"


#include "searchwidget.h"

#include "groupitem.h"
#include "grouptoolitem.h"
#include "grouplistarea.h"

#include <QRegExp>
#include "wechat.h"
#include "groupmembermodel.h"
#include "QXmppMessage.h"

#include "QXmppDataForm.h"

#include <QFile>
#include <QXmlStreamWriter>
#include "singlechat.h"
#include "showmessage.h"
#include "itemofmb.h"

#include <qt_windows.h> //添加windows库

#include <QLibrary>
#include <QDateTime>

extern QString LoginUserName;   //定义在logindialog中
extern QMap<int,WeChat*> openGroupChatMap;
extern QMap<QString, SingleChat *> openSingleChatMap;

QMap<QString, ItemOfMB *> messageBox;

QXmppClient * globalClient;

QString currentChattingJID;

/*
 * 目前这样可以移动整个窗口，先做个样子，后期可以把头部单独拿出来
 * 做一个Dialog 然后重写鼠标事件，再把那个Dialog添加到本Dialog
 * 上来
 */

/*
 * FriendDialog构造函数
 *  首先此Dialog不显示，先显示登录Dialog
 *  确认登录成功之后，显示登录界面
 */

typedef int (__stdcall *FnStartScreenCaptureW)(const wchar_t* szDefaultSavePath, void* pCallBack, UINT_PTR hWndNotice, UINT_PTR noticeMsg, UINT_PTR hwndHideWhenCapture, int autoCapture, int x, int y, int width, int height);
FnStartScreenCaptureW gl_StartScreenCapture = NULL;
typedef int (__stdcall *FnInitScreenCaptureW)(const wchar_t* szAuth);
FnInitScreenCaptureW gl_InitCapture = NULL;

typedef int (__stdcall *FnInitCaptureParamW)(int flag, UINT_PTR flagvalue);
FnInitCaptureParamW gl_InitCaptureParam = NULL;

typedef enum
{
    emPensize = 1,		//设置画笔大小
    emDrawType,			//设置是腾讯风格还是360风格
    emTrackColor,		//自动识别的边框的颜色
    emEditBorderColor,	//文本输入的边框颜色
    emTransparent,		//工具栏的透明度
    emWindowAware,		//设置是否禁用随着DPI放大
    emDetectSubWindowRect,	//是否自动检测子窗口，暂时无用
    emSetSaveName,		//设置保存时的开始文字
    emSetMagnifierBkColor, //设置放大镜的背景色，不设置则透明
    emSetMagnifierLogoText, //设置放大镜上的LOGO字符，可提示快捷键，如： 牛牛截图(CTRL + SHIFT + A)
    emSetNoticeCallback = 19,				//用于设置控件通知信息的回调函数
    emSetWatermarkPictureType,						//设置水印的类型
    emSetWatermarkPicturePath,						//设置水印的路径
    emSetWatermarkTextType,						//设置水印文字的类型
    emSetWatermarkTextValue,						//设置水印文字
    emSetMosaicType,							//设置马赛克类型，1为矩形，2为画线
    emSetToolbarText,							//设置工具栏上的各按钮的tooltip及显示的文字

}ExtendFlagTypeEnum;

FriendDialog * thisPointer; //全局的FriendDialog指针

QString newPicPath;  //定义全局的图片路径

QMap<int, WeChat*> openGroupChatMap;  //打开的群聊窗口

FriendDialog::FriendDialog(QDialog *parent) : QDialog(parent)
{
    setupUi(this);

/*******************初始化托盘相关start*******************/
    m_trayIcon = new QSystemTrayIcon;
    initSysTrayContextMenu();
/*******************初始化托盘相关end*********************/


/********************初始化截屏start*********************************/
    thisPointer = this;

    //编译x86还是x64的 在.pro文件中定义
#ifdef __WINX64__
    QString strDllPath = QCoreApplication::applicationDirPath() + "/NiuniuCapturex64.dll";
#else
    QString strDllPath = QCoreApplication::applicationDirPath() + "/NiuniuCapture.dll";
#endif

    //加载截屏动态库
    QLibrary dll(strDllPath);
    bool isload = dll.load();
    Q_ASSERT(isload);

    //获取导出函数地址
    gl_StartScreenCapture = (FnStartScreenCaptureW)dll.resolve("StartScreenCaptureW");
    gl_InitCapture = (FnInitScreenCaptureW)dll.resolve("InitScreenCaptureW");
    gl_InitCaptureParam = (FnInitCaptureParamW)dll.resolve("InitCaptureParamW");

    //初始化配置
    string authCode = "niuniu";
    string strPwd = GenerateAuthCode(authCode);
    wstring wcsCode = QString(strPwd.c_str()).toStdWString();

    //此函数需要先于其他设置函数调用
    gl_InitCapture(wcsCode.c_str());

    gl_InitCaptureParam(ExtendFlagTypeEnum::emPensize, 2);	//画笔线宽
    gl_InitCaptureParam(ExtendFlagTypeEnum::emDrawType, 0);	//设置放大镜风格：0： 腾讯风格   1： 360风格
    gl_InitCaptureParam(ExtendFlagTypeEnum::emTrackColor, QColor(255,0,0).rgb());	//自动识别的边框颜色
    gl_InitCaptureParam(ExtendFlagTypeEnum::emEditBorderColor, QColor(0,174,255).rgb());	//文字编辑框边框颜色
    gl_InitCaptureParam(ExtendFlagTypeEnum::emTransparent, 240); //设置工具栏窗口透明度

    wstring strSavePath = QString("牛牛截图").toStdWString();
    gl_InitCaptureParam(ExtendFlagTypeEnum::emSetSaveName, (UINT_PTR)strSavePath.c_str()); //设置保存时的开始文字
    gl_InitCaptureParam(ExtendFlagTypeEnum::emSetMagnifierBkColor, QColor(255, 255, 255).rgb()); //设置放大镜的背景色，不设置则透明

    //以下可以设置放大镜上的LOGO文字，如果不设置，默认显示“牛牛截图”
    //gl_InitCaptureParam(ExtendFlagTypeEnum::emSetMagnifierLogoText, (UINT_PTR)"牛牛截图(Ctrl+Shift+A)");
    wstring strMagnifierLogoText = QString("  可通过接口设置名称").toStdWString();
    gl_InitCaptureParam(ExtendFlagTypeEnum::emSetMagnifierLogoText, (UINT_PTR)strMagnifierLogoText.c_str());

    //以下设置工具栏上的各个按钮的tooltip文字以及完成按钮显示的文字信息
    //tipRectangle|tipCircle|tipArrow|tipBrush|tipGlitter|tipMosaic|tipText|tipUndo|tipSave|tipCancel|tipFinish|txtFinish
    wstring strToolbarText = QString("Rectangle|Circle|Arrow|Brush|Glitter|Mosaic|Text|Undo|Save|Cancel|Finish|Finish").toStdWString();
    gl_InitCaptureParam(ExtendFlagTypeEnum::emSetToolbarText, (UINT_PTR)strToolbarText.c_str());
    gl_InitCaptureParam(ExtendFlagTypeEnum::emWindowAware, 1); //此函数必需窗口创建前调用，其等同于如下代码


    gl_InitCaptureParam(ExtendFlagTypeEnum::emSetWatermarkTextValue, (UINT_PTR)"");
    gl_InitCaptureParam(2, 0);	//参数2的意义，0： 腾讯风格   1： 360风格

/********************初始化截屏end***********************************/

    //创建XmppClient
    m_xmppClient = new QXmppClient;

    setAttribute(Qt::WA_QuitOnClose, true);

    /*
     * 隐藏标题栏和保持在最上
     * 保持在最上是因为靠边收起后，还可以激活
     */
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);


    m_showMessage = new ShowMessage;

    groupList = new QList<group_info_t>;

//    this->hide();

    //获取桌面宽度
    m_desktopWidth = QApplication::desktop()->width();

    //隐藏方向设置空
    m_direction = None;

    //自动隐藏默认否
    m_isAutoHide = false;

    this->m_moving  = false;

    m_friendListArea = new FriendListArea;
    m_friendListArea->adjustSize();

    m_groupListArea = new GroupListArea;

    /********************************************************************************************/
    IMToolBox * m_box2 = new IMToolBox;
    m_groupItme = new GroupToolItem("我的群组");
    m_box2->addItem(m_groupItme);

    this->m_groupListArea->addItem(m_box2);
    this->groupAreaLayout->addWidget(m_groupListArea);

    /*******************************************************************************************/

    IMToolBox * m_box1 = new IMToolBox;
    m_item1 = new IMToolItem("title1");

    m_box1->addItem(m_item1);

    m_friendListArea->addItem(m_box1);

    //初始化显示好友
    this->stackedWidget->setCurrentIndex(0);
    this->myLabel_3->setStyleSheet(QStringLiteral("image: url(:/images/icon_contacts_selected.png);"));

    this->friendAreaLayout->addWidget(m_friendListArea);

    connect(this->myLabel, SIGNAL(clicked()), this, SLOT(ShowMinimize()));
    connect(this->myLabel_2, SIGNAL(clicked()), this, SLOT(close()));

    //接收好友等信息
    connect(&this->m_xmppClient->rosterManager(), SIGNAL(rosterReceived()), this, SLOT(rosterReceived()));

    //好友在线状态改变
    connect(&this->m_xmppClient->rosterManager(), SIGNAL(presenceChanged(QString,QString)), this, SLOT(presenceChanged(QString,QString)));

    //好友申请
    connect(this->m_xmppClient, SIGNAL(presenceReceived(QXmppPresence)), this, SLOT(presenceReceived(QXmppPresence)));

    //添加好友
    connect(this->pushButton_addContact, SIGNAL(clicked(bool)), this, SLOT(addContact()));

    //删除好友
    connect(this->m_item1, SIGNAL(removeContact(QString)), this, SLOT(actionRemovecontact(QString)));

    //显示查找好友群组界面
    connect(this->searchButton, SIGNAL(clicked()), this, SLOT(showSearchUserOrGroup()));

    //登录连接后获取群组列表
    connect(this->m_xmppClient, SIGNAL(connected()), this, SLOT(getGroupList()));

    /*************关联单聊、群聊、历史聊天三个按钮******************/

    connect(this->myLabel_3, SIGNAL(clicked()), this, SLOT(showSingleChatList()));

    connect(this->myLabel_4, SIGNAL(clicked()), this, SLOT(showGroupChatList()));

    connect(this->myLabel_5, SIGNAL(clicked()), this, SLOT(showTemporaryChatList()));

    /**********************************************/

    connect(this->m_xmppClient, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(messageReceived(QXmppMessage)));
}

void FriendDialog::mousePressEvent(QMouseEvent *event)
{
    this->m_moving = true;
    this->m_movePosition = event->globalPos() - this->pos();
    return QDialog::mousePressEvent(event);
}

void FriendDialog::mouseReleaseEvent(QMouseEvent *event)
{
    this->m_moving = false;
    return QDialog::mousePressEvent(event);
}

void FriendDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() && Qt::LeftButton &&
            (event->globalPos()-m_movePosition).manhattanLength() > QApplication::startDragDistance())
    {
        this->move(event->globalPos() - m_movePosition);
        return QDialog::mouseMoveEvent(event);
    }
}

FriendDialog::~FriendDialog()
{
    leaveAllRoom();
}

void FriendDialog::ShowMinimize()
{
//    this->showMinimized();
    this->hide();

    QIcon icon = QIcon(":/images/avatar.png");

    m_trayIcon->setIcon(icon);

    m_trayIcon->setToolTip(tr("显示托盘"));

    //给QSystemTrayIcon添加槽函数
    connect(m_trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

    m_trayIcon->setContextMenu(m_sysTrayMenu);
    m_trayIcon->show();
}

void FriendDialog::CloseApp()
{

}


/*
 * 所有程序由此开始
 * 先不显示FriendDlg
 * 显示loginDlg
 */
void FriendDialog::runApp()
{
    m_loginDialog = new LoginDialog(this->m_xmppClient);
    m_loginDialog->show();
    connect(this->m_loginDialog, SIGNAL(loginSucess()), this, SLOT(showFriendDialog()));
    connect(this->m_loginDialog, SIGNAL(loginSucess()), this, SLOT(InitGlobalClient()));
}

void FriendDialog::showFriendDialog()
{
    this->label->setText(LoginUserName);
    this->show();
}

void FriendDialog::InitGlobalClient()
{
    globalClient = this->m_xmppClient;
}

void FriendDialog::rosterReceived()
{
    foreach (const QString &bareJid, this->m_xmppClient->rosterManager().getRosterBareJids()) {
        QString name = this->m_xmppClient->rosterManager().getRosterEntry(bareJid).name();
        if(name.isEmpty())
            name = "-";
        qDebug("example_2_rosterHandling:: Roster received: %s [%s]", bareJid.toStdString().c_str(), qPrintable(name));

        //如果barejid中包含群组类型JID，则过滤掉
        if(!bareJid.contains("conference.im.weitainet.com"), Qt::CaseSensitive)
        {
            qDebug()<<"current add is :  "<<bareJid;
            m_item1->getOrCreateItem(bareJid);
//            continue;
        }

    }

}

void FriendDialog::presenceChanged(const QString &bareJid, const QString &resource)
{
    QMap<QString, QXmppPresence> presences = m_xmppClient->rosterManager().getAllPresencesForBareJid(bareJid);

    this->m_item1->updatePresence(bareJid, presences);
}

void FriendDialog::presenceReceived(const QXmppPresence &presence)
{
    QString reqFrom = presence.from(); //请求来源

    qDebug()<<reqFrom<<"请求";

    QString message;

    int retButton = 0;

    switch (presence.type())
    {
    case QXmppPresence::Subscribe:
        message = "<B>%1</B> wants to subscribe";

        retButton = QMessageBox::question(
                this, "Contact Subscription", message.arg(reqFrom),
                QMessageBox::Yes, QMessageBox::No);

        if(retButton == QMessageBox::Yes)
        {
            QXmppPresence subscribed;
            subscribed.setTo(reqFrom);
            subscribed.setType(QXmppPresence::Subscribed);
            m_xmppClient->sendPacket(subscribed);

            // reciprocal subscription
            QXmppPresence subscribe;
            subscribe.setTo(reqFrom);
            subscribe.setType(QXmppPresence::Subscribe);
            m_xmppClient->sendPacket(subscribe);
        }
        else if(retButton == QMessageBox::No)
        {
            QXmppPresence unsubscribed;
            unsubscribed.setTo(reqFrom);
            unsubscribed.setType(QXmppPresence::Unsubscribed);
            m_xmppClient->sendPacket(unsubscribed);
        }

        break;

    case QXmppPresence::Subscribed:
        message = "<B>%1</B> accepted your request";
        QMessageBox::information(this, "Contact Subscription", message.arg(reqFrom),
                                 QMessageBox::Yes);
        break;
    case QXmppPresence::Unsubscribe:
        message = "<B>%1</B> unsubscribe";
        qDebug()<<message;
        break;
    case QXmppPresence::Unsubscribed:
        message = "<B>%1</B> unsubscribed";
        qDebug()<<message;
        break;

    default:
        break;
    }
}

void FriendDialog::addContact()
{
    bool ok;
    QString bareJid = QInputDialog::getText(this, "Add a jabber contact",
                                            "Contact ID:", QLineEdit::Normal, "", &ok);

    if(!ok)
        return;

//    if(!isValidBareJid(bareJid))
//    {
//        QMessageBox::information(this, "Invalid ID", "Specified ID <I>"+bareJid + " </I> is invalid.");
//        return;
//    }

    if(ok && !bareJid.isEmpty())
    {
        QXmppPresence subscribe;
        subscribe.setTo(bareJid);
        subscribe.setType(QXmppPresence::Subscribe);
        m_xmppClient->sendPacket(subscribe);
    }
}

void FriendDialog::actionRemovecontact(const QString &bareJid)
{
    int answer = QMessageBox::question(this, "Remove contact",
                                                        QString("Do you want to remove the contact <I>%1</I>").arg(bareJid),
                                                        QMessageBox::Yes, QMessageBox::No);
    if(answer == QMessageBox::Yes)
    {
        QXmppRosterIq remove;
        remove.setType(QXmppIq::Set);
        QXmppRosterIq::Item itemRemove;
        itemRemove.setSubscriptionType(QXmppRosterIq::Item::Remove);
        itemRemove.setBareJid(bareJid);
        remove.addItem(itemRemove);

        m_xmppClient->sendPacket(remove);
    }

}

void FriendDialog::showSearchUserOrGroup()
{
    SearchWidget * searchWidget = new SearchWidget;
    searchWidget->show();
}

void FriendDialog::getGroupList()
{
    qDebug()<<LoginUserName<<"username................";
    WTAPI.SelectGroupList(LoginUserName, groupList);

    QList<group_info_t>::iterator group;
    for(group=groupList->begin(); group!=groupList->end(); group++)
    {
        m_groupItme->getOrCreateItem((*group).acGroupName, (*group).nGroupId);
    }

//    allRoomEvents();
    joinAllRoom();


}

void FriendDialog::joinAllRoom()
{
    QXmppMucManager * manager = new QXmppMucManager;
    m_xmppClient->addExtension(manager);

    QList<group_info_t>::iterator group;
    for(group=groupList->begin(); group!=groupList->end(); group++)
    {
        unsigned int groupId = (*group).nGroupId;
        QString roomJID = QString::number(groupId,10)+"@conference.im.weitainet.com";

        qDebug()<<roomJID<<"定向出席房间";

        QXmppMucRoom *room = manager->addRoom(roomJID);
        groupRoomMap.insert(QString::number(groupId,10), room);
        room->setNickName(LoginUserName);

        room->join();

        //request the room config and will receive a signal
        room->requestConfiguration();

        connect(room, SIGNAL(configurationReceived(QXmppDataForm)), this, SLOT(configurationReceived(QXmppDataForm)));

        connect(room, SIGNAL(participantAdded(QString)), this, SLOT(participantAdded(QString)));

        connect(room, SIGNAL(participantRemoved(QString)), this, SLOT(participantRemoved(QString)));

        connect(room, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(messageGroupReceived(QXmppMessage)));


//        bool isJoin = room->join();

//        Q_ASSERT(isJoin);


    }

}

void FriendDialog::leaveAllRoom()
{
    if(!groupRoomMap.empty())
    {
        QMap<QString, QXmppMucRoom*>::iterator iter;

        for(iter = groupRoomMap.begin(); iter != groupRoomMap.end(); iter++)
        {
            QXmppMucRoom * room = iter.value();
            bool isLeave = room->leave();

            Q_ASSERT(isLeave);

            delete room;
        }
    }
}


//before join room , all setting must be setup yet.
//not used
void FriendDialog::allRoomEvents()
{
    if(!groupRoomMap.empty())
    {
        QMap<QString, QXmppMucRoom*>::iterator iter;
        for(iter = groupRoomMap.begin(); iter != groupRoomMap.end(); iter++)
        {
            QXmppMucRoom * room = iter.value();

            connect(room, SIGNAL(participantRemoved(QString)), this, SLOT(participantRemoved(QString)));

            connect(room, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(messageGroupReceived(QXmppMessage)));

            connect(room, SIGNAL(participantAdded(QString)), this, SLOT(participantAdded(QString)));
        }
    }
}

void FriendDialog::participantRemoved(const QString &jid)
{
    QXmppMucRoom * room = qobject_cast<QXmppMucRoom *> (sender());
    QString RoomJID = room->jid();
    //根据JID获取房间号

    QRegExp rx("(\\d+)");
    int pos = 0;
    int c_groupId = 0;
    while((pos = rx.indexIn(RoomJID, pos)) != -1)
    {
        c_groupId = rx.cap(0).toInt();
        pos = rx.matchedLength();
    }

    qDebug()<<"当前房间号为"<<c_groupId;
    qDebug()<<"当前离开的jid为："<<jid;

    QString username = getUserNameFromJID(jid);  //离开房间的username

    changeParticpantStateLeave(c_groupId, username);
}

void FriendDialog::participantAdded(const QString &jid)
{
    QXmppMucRoom * room = qobject_cast<QXmppMucRoom *> (sender());
    QString RoomJID = room->jid();
    //根据JID获取房间号

    QRegExp rx("(\\d+)");
    int pos = 0;
    int c_groupId = 0;
    while((pos = rx.indexIn(RoomJID, pos)) != -1)
    {
        c_groupId = rx.cap(0).toInt();
        pos = rx.matchedLength();
    }

    qDebug()<<"当前房间号为"<<c_groupId;
    qDebug()<<"当前加入的jid为："<<jid;

    QString username = getUserNameFromJID(jid);  //加入房间的username

    changeParticpantStateAdd(c_groupId, username);
}

QString FriendDialog::getUserNameFromJID(const QString &jid)
{
    QString username = jid.section("/",1,1);
    return username;
}

void FriendDialog::changeParticpantStateLeave(int groupId, QString username)
{
    if(openGroupChatMap.contains(groupId))
    {
        WeChat * weChat = openGroupChatMap[groupId];

        QMap<QString, GroupMemberModel *> * groupMemberMap = weChat->getGroupMemberMap();

        GroupMemberModel * model = groupMemberMap->value(username);

        model->setNickName("lixian");
    }

}

void FriendDialog::changeParticpantStateAdd(int groupId, QString username)
{
    if(openGroupChatMap.contains(groupId))
    {
        WeChat * weChat = openGroupChatMap[groupId];

        QMap<QString, GroupMemberModel *> * groupMemberMap = weChat->getGroupMemberMap();

        GroupMemberModel * model = groupMemberMap->value(username);

        model->setNickName(username);
    }

}

/*
 * 根据应用程序当前位置判断朝什么方向隐藏
 */
void FriendDialog::isAutoHide()
{
    QPoint pos = this->pos();

    m_isAutoHide = true;

    if(pos.x() < 2)
    {
        m_direction = Left;
        qDebug()<<"direction left";
    }
    else if(pos.y() < 2)
    {
        m_direction = Up;
        qDebug()<<"direction up";
    }
    else if(pos.x() + this->width() > m_desktopWidth - 2)
    {
        //鼠标当前位置加上应用程序宽度超过桌面宽度，
        //说明朝右隐藏
        m_direction = Right;
        qDebug()<<"direction rigth";
    }
    else
    {
        m_direction = None;
        m_isAutoHide = false;
    }

}

void FriendDialog::showWidget()
{
    QPoint pos = this->pos();

    QPropertyAnimation * animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(100);
    animation->setStartValue(QRect(pos, this->size()));

    QRect rcEnd;

    if(m_direction == Up)
    {
        rcEnd = QRect(this->x(), 0, this->size().width(), this->rect().height());
    }
    else if (m_direction == Left)
    {
        rcEnd = QRect(0, this->y(), this->size().width(), this->rect().height());
    }
    else if (m_direction == Right)
    {
        rcEnd = QRect(m_desktopWidth - this->width(), this->y(), this->size().width(), this->rect().height());
    }

    animation->setEndValue(rcEnd);
    animation->start();
}

/*
 * 采用动画方式移动界面，造成向上收起的效果
 */
void FriendDialog::hideWidget()
{
    QPropertyAnimation * animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(100);
    animation->setStartValue(QRect(this->pos(), this->size()));

    QRect rcEnd;
    if(m_direction == Up)
    {
        rcEnd = QRect(this->x(), -this->height() + 2, this->size().width(), this->size().height());
    }
    else if (m_direction == Left)
    {
        rcEnd = QRect(-this->size().width() + 2, this->y(), this->size().width(), this->size().height());
    }
    else if (m_direction == Right)
    {
        rcEnd = QRect(m_desktopWidth - 2, this->y(), this->size().width(), this->size().height());
    }

    animation->setEndValue(rcEnd);
    animation->start();
}

void FriendDialog::leaveEvent(QEvent *event)
{
    qDebug()<<"leave event";
    isAutoHide();
    if(m_isAutoHide)
    {
        hideWidget();
    }
}

void FriendDialog::enterEvent(QEvent *event)
{
    qDebug()<<"enter event";
    if(m_isAutoHide)
    {
        showWidget();
    }
}

void FriendDialog::showSingleChatList()
{

    this->stackedWidget->setCurrentIndex(0);
    this->myLabel_3->setStyleSheet(QStringLiteral("image: url(:/images/icon_contacts_selected.png);"));
    this->myLabel_4->setStyleSheet(QStringLiteral("image: url(:/images/icon_group_normal.png);"));
    this->myLabel_5->setStyleSheet(QStringLiteral("image: url(:/images/icon_last_normal.png);"));

}

void FriendDialog::showGroupChatList()
{
    this->stackedWidget->setCurrentIndex(1);
    this->myLabel_3->setStyleSheet(QStringLiteral("image: url(:/images/icon_contacts_normal.png);"));
    this->myLabel_4->setStyleSheet(QStringLiteral("image: url(:/images/icon_group_selected.png);"));
    this->myLabel_5->setStyleSheet(QStringLiteral("image: url(:/images/icon_last_normal.png);"));


}

void FriendDialog::showTemporaryChatList()
{
    this->stackedWidget->setCurrentIndex(2);
    this->myLabel_3->setStyleSheet(QStringLiteral("image: url(:/images/icon_contacts_normal.png);"));
    this->myLabel_4->setStyleSheet(QStringLiteral("image: url(:/images/icon_group_normal.png);"));
    this->myLabel_5->setStyleSheet(QStringLiteral("image: url(:/images/icon_last_selected.png);"));
}

void FriendDialog::configurationReceived(const QXmppDataForm &configuration)
{
    qDebug()<<"receive the room configuration and set the config";
    QXmppMucRoom * room = qobject_cast<QXmppMucRoom *> (sender());

    QString * str = new QString;
    QXmlStreamWriter writer(str);

    configuration.toXml(&writer);

//    qWarning()<<*str;

    QXmppDataForm *configure = new QXmppDataForm;
    configure->setType(QXmppDataForm::Submit);

    room->setConfiguration(*configure);

    bool isjoin = room->isJoined();
    Q_ASSERT(isjoin);
    //print the configuration


}

void FriendDialog::messageReceived(const QXmppMessage &message)
{
    //this is a simple message receive example.
    //there are many conditions to judge.
    qDebug()<<"this is single chat:"<<message.body();

    QString messageBody = message.body();
    QString messageFrom = message.from();

    QString bareJID = messageFrom.section("/", 0, 0);

    qDebug()<<"the seciton jid is:"<<bareJID;

    //不知道为什么单人聊天也能接收到群组信息了
    if(bareJID.contains("conference.im.weitainet.com", Qt::CaseSensitive))
    {
        return ;
    }

    //说明此对话窗口已经被打开
    if(openSingleChatMap.contains(bareJID))
    {
        SingleChat * singleChat = openSingleChatMap[bareJID];
        singleChat->setChatText(bareJID + ":");
        singleChat->setChatContent(messageBody);
    }
    else
    {
        if(messageBox.contains(bareJID))
        {
            //说明有未读的信息
            ItemOfMB * item = messageBox[bareJID];
            //
            int messageCount = item->messageCount();
            item->setMessageCount(++messageCount);

            m_showMessage->adjustSize();
            m_showMessage->update();
            m_showMessage->show();
        }
        else
        {
            //没有未读消息则创建
            ItemOfMB * item = new ItemOfMB;
            item->setUsername(bareJID);
            item->setMessageCount(1);

            messageBox[bareJID] = item;

            m_showMessage->addItem(item);
//            m_showMessage->resize(m_showMessage->sizeHint());
            m_showMessage->adjustSize();
            m_showMessage->update();
            m_showMessage->show();

        }
    }
    //如果没有打开的话，就会用m_showMessage显示

}

void FriendDialog::messageGroupReceived(const QXmppMessage &message)
{   
//    qDebug()<<"this is group chat:"<<message.body();

    QString messageBody = message.body();
    QString messageFrom = message.from();

    //还得添加一层过滤，如果群内消息是自己发送的，不显示
    if(messageFrom.contains(LoginUserName, Qt::CaseSensitive))
    {
        return ;
    }

    qDebug()<<"the group chat jid is: "<<messageFrom;

    QString groupIdstr = messageFrom.section("@", 0, 0);
    int groupid = groupIdstr.toInt();
    QString bareJID = messageFrom.section("/", 0, 0);

    //说明此对话窗口已经被打开
    if(openGroupChatMap.contains(groupid))
    {
        WeChat * weChat = openGroupChatMap[groupid];

        weChat->setChatContent("\r\n" + messageFrom + ": " + "\r\n");
        weChat->setChatContent("    " + messageBody + "\r\n");

    }
    else
    {
        if(messageBox.contains(bareJID))
        {
            //说明有未读的信息
            ItemOfMB * item = messageBox[bareJID];
            //
            int messageCount = item->messageCount();
            item->setMessageCount(++messageCount);

            m_showMessage->update();
            m_showMessage->show();
        }
        else
        {
            //没有未读消息则创建
            ItemOfMB * item = new ItemOfMB;
            item->setUsername(bareJID);
            item->setMessageCount(1);

            messageBox[bareJID] = item;

            m_showMessage->addItem(item);
            m_showMessage->adjustSize();
            m_showMessage->update();
            m_showMessage->show();
        }
    }
    //如果没有打开的话，就会用m_showMessage显示

}

void FriendDialog::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
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

void FriendDialog::initSysTrayContextMenu()
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

string FriendDialog::GenerateAuthCode(string strAuthCode)
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyyMd");
    qDebug()<<str;
    string strPwd = str.toStdString()+strAuthCode;

    return MD5Encode((char*)strPwd.c_str());
}

