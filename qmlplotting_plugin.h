#ifndef QMLPLOTTING_PLUGIN_H
#define QMLPLOTTING_PLUGIN_H

#include <QQmlExtensionPlugin>

class QmlPlottingPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // QMLPLOTTING_PLUGIN_H

