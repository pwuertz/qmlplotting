import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QmlPlotting 2.0 as QmlPlotting

ApplicationWindow {
    id: approot
    visible: true
    title: "QmlPlotting Demo"
    minimumWidth: 800
    minimumHeight: 600

    // Demonstration pages
    ListModel {
        id: demoPages
        ListElement { name: "2D Data"; source: "qrc:/pages/ColormappedImage.qml" }
        ListElement { name: "XY Plot"; source: "qrc:/pages/XYPlot.qml"}
        ListElement { name: "Containers"; source: "qrc:/pages/Containers.qml"}
    }
    header: ToolBar {
        focus: true
        Keys.onEscapePressed: { approot.close(); }
        RowLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: Qt.application.font.pixelSize
            Label { text: approot.title }
            ComboBox {
                id: pageSelector
                textRole: "name"
                model: demoPages
                implicitWidth: 10 * Qt.application.font.pixelSize
            }
        }
    }
    Loader {
        anchors.fill: parent
        source: demoPages.get(pageSelector.currentIndex).source
    }
}
