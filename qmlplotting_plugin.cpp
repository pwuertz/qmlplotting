#include "qmlplotting_plugin.h"
#include "colormappedimage.h"
#include "datacontainer.h"

#include <qqml.h>

void QmlPlottingPlugin::registerTypes(const char *uri)
{
    // @uri qao.qmlplotting
    qmlRegisterType<ColormappedImage>(uri, 1, 0, "ColormappedImage");
    qmlRegisterType<DataContainer>(uri, 1, 0, "DataContainer");
}
