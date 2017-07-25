TEMPLATE = lib
TARGET = QmlPlotting
QT += qml quick
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = qmlplotting

# Input
SOURCES += \
    src/qmlplotting_plugin.cpp \
    src/colormappedimage.cpp \
    src/dataclient.cpp \
    src/sliceplot.cpp \
    src/datasource.cpp \
    src/xyplot.cpp \
    src/qsgdatatexture.cpp

HEADERS += \
    src/qmlplotting_plugin.h \
    src/colormappedimage.h \
    src/dataclient.h \
    src/sliceplot.h \
    src/datasource.h \
    src/xyplot.h \
    src/qsgdatatexture.h

QML_FILES += \
    ZoomPanImage.qml \
    ZoomPanArea.qml \
    SelectionBox.qml \
    SelectionBoxImg.qml \
    PlotArea.qml

OTHER_FILES = qmldir \
    test/test_image.qml \
    test/test_xyplot.qml \
    test/test_plotarea.qml \
    test/test_plotarea2.qml \
    test/Window.qml


PREFIX = $$(PREFIX)
isEmpty(PREFIX) {
    PREFIX = $$[QT_INSTALL_QML]
}

qmldir.files = qmldir $$QML_FILES
installPath = $$PREFIX/$$replace(uri, \\., /)
qmldir.path = $$installPath
target.path = $$installPath
INSTALLS += target qmldir

develop.target = develop
develop.commands = $(SYMLINK) $$OUT_PWD $$installPath

QMAKE_EXTRA_TARGETS += develop
