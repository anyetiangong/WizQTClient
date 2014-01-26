#include "wizserviceplugin.h"

#include <QtPlugin>
#include <QDebug>
#include <QDateTime>

#include "avatar.h"

using namespace WizService;
using namespace WizService::Internal;

WizServicePlugin::WizServicePlugin()
{
}

WizServicePlugin::~WizServicePlugin()
{
}

bool WizServicePlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    new WizService::AvatarHost();

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
