import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QmlPlotting 2.0 as QmlPlotting

/**
  ColormappedImage example
  */

Page {
    id: root

    // QmlPlotting colormap image with default test data
    QmlPlotting.ColormappedImage {
        id: image
        anchors.fill: parent
        colormap: colorMaps.get(0).name
        minimumValue: rangeSlider.first.value
        maximumValue: rangeSlider.second.value
        dataSource: QmlPlotting.DataSource {}
        Component.onCompleted: dataSource.setTestData2D();
    }

    // Image value range editing
    Pane {
        anchors.fill: rangeGroup
        anchors.margins: -Qt.application.font.pixelSize / 2
        opacity: 0.8
    }
    ColumnLayout {
        id: rangeGroup
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: Qt.application.font.pixelSize
        Text {
            readonly property string value1: rangeSlider.first.value.toFixed(2)
            readonly property string value2: rangeSlider.second.value.toFixed(2)
            text: "Value range " + value1 + " to " + value2
        }
        RangeSlider {
            id: rangeSlider
            from: -1
            to: 2
            first.value: 0.
            second.value: 1.
            implicitWidth: .4 * root.width
        }
    }

    // Colormap selection
    ListModel {
        id: colorMaps
        ListElement { name: "viridis" }
        ListElement { name: "bwr" }
        ListElement { name: "ferrugineus" }
        ListElement { name: "wjet" }
        ListElement { name: "jet" }
        ListElement { name: "hot" }
        ListElement { name: "gray" }
    }
    Pane {
        anchors.fill: colorGroup
        anchors.margins: -Qt.application.font.pixelSize / 2
        opacity: 0.8
    }
    ColumnLayout {
        id: colorGroup
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: Qt.application.font.pixelSize
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Colormaps"
        }
        Repeater {
            model: colorMaps
            Button {
                Layout.alignment: Qt.AlignHCenter
                text: model.name
                onClicked: { image.colormap = model.name; }
            }
        }
    }
}
