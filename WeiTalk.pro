#-------------------------------------------------
#
# Project created by QtCreator 2017-02-15T17:45:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WeiTalk
TEMPLATE = app


SOURCES += main.cpp \
    logindialog.cpp \
    frienddialog.cpp \
    frienditem.cpp \
    imtoolitem.cpp \
    imtoolbox.cpp \
    listbutton.cpp \
    friendlistarea.cpp

HEADERS  += \
    logindialog.h \
    frienddialog.h \
    frienditem.h \
    imtoolitem.h \
    imtoolbox.h \
    listbutton.h \
    friendlistarea.h

FORMS += \
    login.ui \
    frienddialog.ui \
    frienditem.ui \
    friendlistarea.ui

RESOURCES += \
    faces.qrc \
    images.qrc
