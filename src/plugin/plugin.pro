# QML Plugin Extention
TARGET = qmlplottingplugin
QT += qml quick
CONFIG += exceptions c++14
include($$PWD/../qmlplotting/qmlplotting.pri)
SOURCES += $$PWD/plugin.cpp

# QML Resources
QML_FILES = \
    $$PWD/ZoomPanImage.qml \
    $$PWD/ZoomPanArea.qml \
    $$PWD/SelectionBox.qml \
    $$PWD/SelectionBoxImg.qml \
    $$PWD/PlotArea.qml

# QML Module
TARGETPATH = QmlPlotting
IMPORT_VERSION = 1.0
CONFIG += no_cxx_module
load(qml_plugin)

# Copy QML_FILES to build (how to handle non-shadow builds?)
for(FILE, QML_FILES) copy_qmlfiles.commands += $(COPY) $$system_quote($$system_path($$FILE)) $$system_quote($$system_path($$DESTDIR)) $$escape_expand(\n\t)
first.depends = $(first) copy_qmlfiles
export(first.depends)
export(copy_qmlfiles.commands)
QMAKE_EXTRA_TARGETS += first copy_qmlfiles

# Optional target: Symlink for development
develop.target = develop
develop.commands = $(SYMLINK) $$system_quote($$system_path($$OUT_PWD/../../qml/QmlPlotting)) $$[QT_INSTALL_QML]
QMAKE_EXTRA_TARGETS += develop
