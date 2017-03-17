#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QDir>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLibrary>
#include <QDateTime>
#include <qt_windows.h>
#include <QClipboard>


//typedef unsigned int UINT_PTR;

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

MainWindow * thisPointer;

std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen)
{
    int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
    if(nSize <= 0) return L"";

    WCHAR *pwszDst = new WCHAR[nSize+1];
    if( NULL == pwszDst) return L"";

    MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc, nLen, pwszDst, nSize);
    pwszDst[nSize] = 0;

    if( pwszDst[0] == 0xFEFF)                    // skip Oxfeff
        for(int i = 0; i < nSize; i ++)
            pwszDst[i] = pwszDst[i+1];

    wstring wcharString(pwszDst);
    delete[] pwszDst;

    return wcharString;
}


std::wstring s2ws(const string& s)
{
    return Ansi2WChar(s.c_str(),s.size());
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    thisPointer = this;

    QString strDllPath = QCoreApplication::applicationDirPath() + "/NiuniuCapturex64.dll";
  //QDir::currentPath();
    qDebug()<<strDllPath;

//    QMessageBox::information(this, "title", strDllPath, QMessageBox::Ok);

    QLibrary dll(strDllPath);
    bool isload = dll.load();
    Q_ASSERT(isload);

    gl_StartScreenCapture = (FnStartScreenCaptureW)dll.resolve("StartScreenCaptureW");

    gl_InitCapture = (FnInitScreenCaptureW)dll.resolve("InitScreenCaptureW");
    qDebug()<<int(gl_InitCapture);

    gl_InitCaptureParam = (FnInitCaptureParamW)dll.resolve("InitCaptureParamW");

    string authCode = "niuniu";
    string strPwd = GenerateAuthCode(authCode);

//    wstring wcsCode = s2ws(strPwd);
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

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(btnClicked()));
    connect(this, SIGNAL(captureFinished(int)), this, SLOT(OnCaptureFinish(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

string MainWindow::GenerateAuthCode(string strAuthCode)
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyyMd");
    qDebug()<<str;
    string strPwd = str.toStdString()+strAuthCode;

    return MD5Encode((char*)strPwd.c_str());
}



void CaptureNotice(int nType, int x, int y, int width, int height, const char *szInfo)
{
    if (nType == 1)	//表示截图完成
    {
//        qDebug()<<QString(szInfo);
        qDebug()<<"width:"<<width<<"heigth:"<<height;
        QClipboard * board = QApplication::clipboard();
        thisPointer->ui->label->setPixmap(board->pixmap().scaled(200,200));
//        thisPointer->ui->label->setPixmap(board->pixmap());
    }
    else if(nType == 2)	//表示取消截图
    {

    }
    else		//保存截图
    {

    }
//    ::PostMessage(gl_hWnd, WM_USER + 1111, 1, nType);
    emit thisPointer->captureFinished(nType);
}

void MainWindow::OnCaptureFinish(int type)
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

void MainWindow::btnClicked()
{
    gl_InitCaptureParam(ExtendFlagTypeEnum::emSetWatermarkTextValue, (UINT_PTR)"");

    gl_InitCaptureParam(2, 0);	//参数2的意义，0： 腾讯风格   1： 360风格
    //gl_InitCaptureParam(24, bCheck ? 1 : 2);
    //通过回调函数来通知截图完成事件
//    gl_hWnd = m_hWnd;
    const wchar_t* szSavePath = (QString("c:\\测试一下\\test.jpg").toStdWString().c_str());
    gl_StartScreenCapture(szSavePath, CaptureNotice, 0, 0, 0, 0, 0, 0, 0, 0);

}
