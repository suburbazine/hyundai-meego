#ifndef POPUPPLUGIN_PLUGIN_H
#define POPUPPLUGIN_PLUGIN_H

#include <QtDeclarative/QDeclarativeExtensionPlugin>

class PopUpPluginPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void registerTypes(const char *uri);
};

#endif // POPUPPLUGIN_PLUGIN_H

