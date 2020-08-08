#ifndef QWERTYKEYPADWIDGET_PLUGIN_H
#define QWERTYKEYPADWIDGET_PLUGIN_H

#include <QtDeclarative/QDeclarativeExtensionPlugin>

class QwertyKeypadWidgetPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void registerTypes(const char *uri);
};

#endif // QWERTYKEYPADWIDGET_PLUGIN_H

