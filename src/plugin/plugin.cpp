#include "../qmlplotting/colormappedimage.h"
#include "../qmlplotting/datasource.h"
#include "../qmlplotting/sliceplot.h"
#include "../qmlplotting/xyplot.h"
#include "../qmlplotting/plotgroup.h"

#include <QQmlExtensionPlugin>
#include <QQmlEngine>


class QmlPlottingPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    QmlPlottingPlugin(QObject* parent = nullptr) : QQmlExtensionPlugin(parent) {}

    void registerTypes(const char *uri) Q_DECL_OVERRIDE
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QmlPlotting"));
        Q_UNUSED(uri);

        qmlRegisterType<ColormappedImage>(uri, 2, 0, "ColormappedImage");
        qmlRegisterType<DataSource>(uri, 2, 0, "DataSource");
        qmlRegisterType<SlicePlot>(uri, 2, 0, "SlicePlot");
        qmlRegisterType<XYPlot>(uri, 2, 0, "XYPlot");
        qmlRegisterType<PlotGroup>(uri, 2, 0, "PlotGroup");
    }
};

#include "plugin.moc"
