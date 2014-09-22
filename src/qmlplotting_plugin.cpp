#include "qmlplotting_plugin.h"
#include "colormappedimage.h"
#include "datasource.h"
#include "sliceplot.h"

#include <qqml.h>

void QmlPlottingPlugin::registerTypes(const char *uri)
{
    // @uri qmlplotting
    qmlRegisterType<ColormappedImage>(uri, 1, 0, "ColormappedImage");
    qmlRegisterType<DataSource2D>(uri, 1, 0, "DataSource2D");
    qmlRegisterType<SlicePlot>(uri, 1, 0, "SlicePlot");
}
