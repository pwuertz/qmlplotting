TARGET = qmlplottingplugin
TARGETPATH = QmlPlotting
IMPORT_VERSION = 1.0
QT += qml quick

DEFINES += QTQMLPLOTTING_LIBRARY

include($$PWD/../qmlplotting/qmlplotting.pri)
SOURCES += $$PWD/plugin.cpp

qmlplotting.files = \
    $$PWD/ZoomPanImage.qml \
    $$PWD/ZoomPanArea.qml \
    $$PWD/SelectionBox.qml \
    $$PWD/SelectionBoxImg.qml \
    $$PWD/PlotArea.qml
qmlplotting.prefix = /QmlPlotting
RESOURCES += qmlplotting

CONFIG += no_cxx_module
load(qml_plugin)

CONFIG += exceptions c++14
