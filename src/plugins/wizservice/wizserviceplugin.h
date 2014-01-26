#ifndef WIZSERVICEPLUGIN_H
#define WIZSERVICEPLUGIN_H

#include <extensionsystem/iplugin.h>

namespace Core {
namespace Internal {

class WizServicePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "wiz.plugin.system.internal.wizservice")

public:
    WizServicePlugin();
    ~WizServicePlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage = 0);
    void extensionsInitialized();
    bool delayedInitialize();
};

} // namespace Internal
} // namespace Core

#endif // WIZSERVICEPLUGIN_H
