#include "../qmlplotting/colormappedimage.h"
#include "../qmlplotting/datasource.h"
#include "../qmlplotting/sliceplot.h"
#include "../qmlplotting/xyplot.h"

#include <QQmlExtensionPlugin>
#include <QQmlEngine>


class QmlPlottingPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) Q_DECL_OVERRIDE
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QmlPlotting"));
        Q_UNUSED(uri);

        qmlRegisterType<ColormappedImage>(uri, 1, 0, "ColormappedImage");
        qmlRegisterType<DataSource>(uri, 1, 0, "DataSource");
        qmlRegisterType<SlicePlot>(uri, 1, 0, "SlicePlot");
        qmlRegisterType<XYPlot>(uri, 1, 0, "XYPlot");
    }
};

#include "plugin.moc"
