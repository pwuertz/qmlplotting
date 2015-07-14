import QtQuick 2.0
import qmlplotting 1.0

Rectangle {
    width: 900; height: 500
    color: "gray"

    DataSource {
        id: source1
        objectName: "source1"
        Component.onCompleted: setTestData1D()
    }
    DataSource {
        id: source2
        objectName: "source2"
        Component.onCompleted: setTestData1D()
    }

    Rectangle {
        id: plot_box
        x: 90; y: 35
        width: plotarea.width + 2; height: plotarea.height + 2 + plot_box_top.height
        radius: 4
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#dddddd" }
            GradientStop { position: 1.0; color: "#ffffff" }
        }
        Rectangle {
            id: plot_box_top
            width: parent.width; height: plot_box_label.height + 4
            radius: 4
            gradient: Gradient {
                GradientStop { position: 1.0; color: "#ffffff" }
                GradientStop { position: 0.0; color: "#dddddd" }
            }
            Text {
                x: 4; y: 2
                id: plot_box_label
                text: "Plot Area"
            }
            MouseArea {
                anchors.fill: parent
                drag.target: plot_box
                drag.axis: Drag.XandYAxis
            }
        }

        PlotArea {
            id: plotarea
            width: 800; height: 400
            y: plot_box_top.height
            x: 1

            axesBackgroundColor: "transparent"
            axesBorderColor: "#33000000"
            axesBorderWidth: 4

            plotItems: [
                XYPlot {
                    id: xyplot1
                    objectName: "xyplot1"
                    dataSource: source1
                    lineEnabled: true
                    lineWidth: 2
                    lineColor: "#8888cc"
                    markerEnabled: false
                },
                XYPlot {
                    id: xyplot2
                    objectName: "xyplot2"
                    dataSource: source2
                    lineEnabled: true
                    lineWidth: 2
                    lineColor: "#cc8888"
                    markerEnabled: false
                }
            ]
        }
    }

}
