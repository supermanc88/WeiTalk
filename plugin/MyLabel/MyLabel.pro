CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(mylabelplugin)
TEMPLATE    = lib

HEADERS     = mylabelplugin.h
SOURCES     = mylabelplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(mylabel.pri)
