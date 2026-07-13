#ifndef MD3COREPLUGIN_H
#define MD3COREPLUGIN_H

#include <QQmlExtensionPlugin>

class Md3CorePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
    void initializeEngine(QQmlEngine *engine, const char *uri) override;
};

#endif // MD3COREPLUGIN_H
