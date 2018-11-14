import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QmlPlotting 2.0 as QmlPlotting

/**
  XYPlot example
  */

Page {
    id: root

    ColumnLayout {
        anchors.fill: parent
        anchors.rightMargin: 5
        // Plot axes for XY plot demonstration
        QmlPlotting.Axes {
            Layout.fillHeight: true
            Layout.fillWidth: true
            xLabel: "X Axis"
            yLabel: "Y Axis"
            // Define plot group containing zoom/pan tool and XY plot item
            plotGroup: QmlPlotting.PlotGroup {
                viewRect: Qt.rect(-1, -.1, 2, 1.2)
                plotItems: [
                    QmlPlotting.ZoomPanTool {
                        maxViewRect: Qt.rect(-2., -1., 4., 3.)
                        minimumSize: Qt.size(0.1, 0.1)
                    },
                    // XY plot item with default test data
                    QmlPlotting.XYPlot {
                        id: xyPlot
                        dataSource: QmlPlotting.DataSource {}
                        Component.onCompleted: dataSource.setTestData1D();
                        lineColor: Qt.hsva(hueSlider.hue, .7, .7, .7)
                        lineWidth: lineWidthSlider.value
                        markerColor: Qt.hsva(hueSlider.hue, .7, .7, .6)
                        markerSize: markerSizeSlider.value
                    }
                ]
            }
        }
        // Controls for changing plot properties
        RowLayout {
            Layout.margins: Qt.application.font.pixelSize
            spacing: Qt.application.font.pixelSize / 2
            // Change hue values
            Slider {
                id: hueSlider
                readonly property real hue: value * (1. / to)
                from: 0; to: 100; stepSize: 1; value: 55
                ToolTip.visible: hovered || pressed
                ToolTip.text: "Hue value"
            }
            // Change line width
            Slider {
                id: lineWidthSlider
                from: 1; to: 10; stepSize: 1; value: 3
                ToolTip.visible: hovered || pressed
                ToolTip.text: "Line width " + value
            }
            // Change marker size
            Slider {
                id: markerSizeSlider
                from: 1; to: 50; stepSize: 1; value: 15
                ToolTip.visible: hovered || pressed
                ToolTip.text: "Marker size " + value
            }
        }
    }
}
