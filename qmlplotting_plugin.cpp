#include "qmlplotting_plugin.h"
#include "colormappedimage.h"
#include "datacontainer.h"
#include "lineplot.h"

#include <qqml.h>

void QmlPlottingPlugin::registerTypes(const char *uri)
{
    // @uri qmlplotting
    qmlRegisterType<ColormappedImage>(uri, 1, 0, "ColormappedImage");
    qmlRegisterType<DataContainer>(uri, 1, 0, "DataContainer");
    qmlRegisterType<LinePlot>(uri, 1, 0, "LinePlot");
}
