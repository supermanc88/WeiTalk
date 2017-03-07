#-------------------------------------------------
#
# Project created by QtCreator 2017-02-15T17:45:08
#
#-------------------------------------------------
QXMPP_INCLUDEPATH = $$PWD/qxmpp/base $$PWD/qxmpp/client $$PWD/qxmpp/server
QXMPP_LIB = $$PWD/qxmpp/LIB/libqxmpp_d0.a

INCLUDEPATH += $$QXMPP_INCLUDEPATH
LIBS += $$QXMPP_LIB

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = WeiTalk
TEMPLATE = app


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
    groupitem.cpp

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
    groupitem.h


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
    groupitem.ui

RESOURCES += \
    faces.qrc \
    images.qrc


