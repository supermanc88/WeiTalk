#-------------------------------------------------
#
# Project created by QtCreator 2017-02-15T17:45:08
#
#-------------------------------------------------

QXMPP_INCLUDEPATH = $$PWD/qxmpp/base $$PWD/qxmpp/client $$PWD/qxmpp/server
QXMPP_DEBUG_LIB = $$PWD/qxmpp/LIB/DebugLib
QXMPP_RELEASE_LIB = $$PWD/qxmpp/LIB/ReleaseLib

INCLUDEPATH += $$QXMPP_INCLUDEPATH

CONFIG(debug, debug|release) {
    win32{
        LIBS += $$QXMPP_DEBUG_LIB/qxmpp_d0.lib
#        LIBS += $$QXMPP_DEBUG_LIB/libqxmpp_d0.a
    }else{
        LIBS += -L$$QXMPP_DEBUG_LIB -lqxmpp_d
    }
    DESTDIR += $$PWD/qxmpp/LIB/DebugLib/
} else {
    win32{
        LIBS += -L$$QXMPP_RELEASE_LIB/qxmpp0.lib
#        LIBS += -L$$QXMPP_RELEASE_LIB/libqxmpp0.a
    }else{
        LIBS += -L$$QXMPP_RELEASE_LIB -lqxmpp
    }

    DESTDIR += $$PWD/qxmpp/LIB/ReleaseLib/
}


QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network xml

TARGET = WeiTalk
TEMPLATE = app

DEFINES += __WINX64__


SOURCES += main.cpp \
    logindialog.cpp \
    frienddialog.cpp \
    frienditem.cpp \
    imtoolitem.cpp \
    imtoolbox.cpp \
    listbutton.cpp \
    friendlistarea.cpp \
    singlechat.cpp \
    wechat.cpp \
    slidenavigation.cpp \
    checkmsg.cpp \
    searchwidget.cpp \
    mylabel.cpp \
    weitalkserverapi.cpp \
    userorgroupitem.cpp \
    grouplistarea.cpp \
    grouptoolitem.cpp \
    groupitem.cpp \
    groupmembermodel.cpp \
    showmessage.cpp \
    itemofmb.cpp \
    MD5.cpp \
    sendpicthread.cpp

HEADERS  += \
    logindialog.h \
    frienddialog.h \
    frienditem.h \
    imtoolitem.h \
    imtoolbox.h \
    listbutton.h \
    friendlistarea.h \
    singlechat.h \
    wechat.h \
    slidenavigation.h \
    checkmsg.h \
    searchwidget.h \
    mylabel.h \
    weitalkserverapi.h \
    userorgroupitem.h \
    grouplistarea.h \
    grouptoolitem.h \
    groupitem.h \
    groupmembermodel.h \
    showmessage.h \
    itemofmb.h \
    MD5.h \
    sendpicthread.h


FORMS += \
    login.ui \
    frienddialog.ui \
    frienditem.ui \
    friendlistarea.ui \
    singlechat.ui \
    wechat.ui \
    checkmsg.ui \
    searchwidget.ui \
    userorgroupitem.ui \
    grouplistarea.ui \
    groupitem.ui \
    groupmembermodel.ui \
    showmessage.ui \
    itemofmb.ui

RESOURCES += \
    faces.qrc \
    images.qrc

DISTFILES +=


