#include "wizserviceplugin.h"

#include <QtPlugin>
#include <QDebug>
#include <QDateTime>

using namespace Core;
using namespace Core::Internal;

WizServicePlugin::WizServicePlugin()
{
}

WizServicePlugin::~WizServicePlugin()
{
}

bool WizServicePlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    return true;
}

void WizServicePlugin::extensionsInitialized()
{
}

bool WizServicePlugin::delayedInitialize()
{
    return true;
}

Q_EXPORT_PLUGIN(WizServicePlugin)
