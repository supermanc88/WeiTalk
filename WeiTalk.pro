#-------------------------------------------------
#
# Project created by QtCreator 2017-02-15T17:45:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WeiTalk
TEMPLATE = app

#INCLUDEPATH += C:\Users\chengheming\Documents\QTProject\WeiTalk\slidenavigation

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
    slidenavigation/slidenavigation.cpp \
    slidenavigation.cpp \
    checkmsg.cpp \
    searchwidget.cpp \
    mylabel.cpp

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
    slidenavigation/slidenavigation.h \
    slidenavigation.h \
    checkmsg.h \
    searchwidget.h \
    mylabel.h


FORMS += \
    login.ui \
    frienddialog.ui \
    frienditem.ui \
    friendlistarea.ui \
    singlechat.ui \
    wechat.ui \
    checkmsg.ui \
    searchwidget.ui

RESOURCES += \
    faces.qrc \
    images.qrc


