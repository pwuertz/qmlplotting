#include "qmlplotting_plugin.h"
#include "colormappedimage.h"
#include "datasource.h"
#include "sliceplot.h"
#include "xyplot.h"

#include <qqml.h>

void QmlPlottingPlugin::registerTypes(const char *uri)
{
    // @uri qmlplotting
    qmlRegisterType<ColormappedImage>(uri, 1, 0, "ColormappedImage");
    qmlRegisterType<DataSource>(uri, 1, 0, "DataSource");
    qmlRegisterType<SlicePlot>(uri, 1, 0, "SlicePlot");
    qmlRegisterType<XYPlot>(uri, 1, 0, "XYPlot");
}
