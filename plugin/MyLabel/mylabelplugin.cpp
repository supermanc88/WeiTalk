#include "mylabel.h"
#include "mylabelplugin.h"

#include <QtPlugin>

MyLabelPlugin::MyLabelPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void MyLabelPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool MyLabelPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *MyLabelPlugin::createWidget(QWidget *parent)
{
    return new MyLabel(parent);
}

QString MyLabelPlugin::name() const
{
    return QLatin1String("MyLabel");
}

QString MyLabelPlugin::group() const
{
    return QLatin1String("");
}

QIcon MyLabelPlugin::icon() const
{
    return QIcon();
}

QString MyLabelPlugin::toolTip() const
{
    return QLatin1String("");
}

QString MyLabelPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool MyLabelPlugin::isContainer() const
{
    return false;
}

QString MyLabelPlugin::domXml() const
{
    return QLatin1String("<widget class=\"MyLabel\" name=\"myLabel\">\n</widget>\n");
}

QString MyLabelPlugin::includeFile() const
{
    return QLatin1String("mylabel.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(mylabelplugin, MyLabelPlugin)
#endif // QT_VERSION < 0x050000
