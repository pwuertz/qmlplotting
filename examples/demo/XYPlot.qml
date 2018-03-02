import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2
import QmlPlotting 2.0 as QmlPlotting

/**
  XYPlot example
  */

Page {
    id: root
    ColumnLayout {
        anchors.fill: parent

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
                    QmlPlotting.ZoomPanTool {},
                    // XY plot item with default test data
                    QmlPlotting.XYPlot {
                        id: xyPlot
                        dataSource: QmlPlotting.DataSource {}
                        Component.onCompleted: dataSource.setTestData1D();
                        lineColor: "lightsteelblue"
                        lineWidth: lineWidthSlider.value
                        markerColor: "steelblue"
                        markerSize: markerSizeSlider.value
                    }
                ]
            }
        }

        // Controls for changing plot properties
        RowLayout {
            Layout.margins: Qt.application.font.pixelSize
            spacing: Qt.application.font.pixelSize / 2

            // Pick color for line and markers
            Repeater {
                model: ["lineColor", "markerColor"]
                Button {
                    text: modelData
                    onClicked: {
                        colorDialog.targetProperty = modelData;
                        colorDialog.color = xyPlot[modelData];
                        colorDialog.open();
                    }
                }
            }
            // Change line width
            Slider {
                id: lineWidthSlider
                from: 1; to: 10; stepSize: 1; value: 2
            }
            // Change marker size
            Slider {
                id: markerSizeSlider
                from: 1; to: 50; stepSize: 1; value: 10
            }
        }
    }

    ColorDialog {
        id: colorDialog
        title: "Choose a color"
        property string targetProperty
        showAlphaChannel: true
        onCurrentColorChanged: { xyPlot[targetProperty] = currentColor; }
    }
}
