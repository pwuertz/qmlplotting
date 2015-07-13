import QtQuick 2.0
import qmlplotting 1.0

Rectangle {
    width: 640; height: 512
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
        x: 5; y: 35
        width: plotarea.width + 2; height: plotarea.height + 2 + plot_box_top.height
        border.width: 1
        border.color: "black"
        color: "#ccffffff"
        radius: 4

        ZoomPanArea {
            id: plotarea
            width: 630; height: 200
            y: plot_box_top.height
            x: 1

            XYPlot {
                id: xyplot1
                objectName: "xyplot"
                anchors.fill: parent
                dataSource: source1
                viewRect: parent.viewRect
                lineEnabled: true
                lineWidth: 1.
                lineColor: "#8888cc"
                markerEnabled: false
            }
            XYPlot {
                id: xyplot2
                objectName: "xyplot"
                anchors.fill: parent
                dataSource: source2
                viewRect: parent.viewRect
                lineEnabled: true
                lineWidth: 1.
                lineColor: "#cc8888"
                markerEnabled: false
            }
            SelectionBox {
                id: selection_user
            }
        }

        Rectangle {
            id: plot_box_top
            width: parent.width; height: plot_box_label.height + 4
            border.width: 1
            border.color: "black"
            color: "#77ffffff"
            radius: 4
            Text {
                x: 4; y: 2
                id: plot_box_label
                text: "XY Plot"
            }
            MouseArea {
                anchors.fill: parent
                drag.target: plot_box
                drag.axis: Drag.XandYAxis
            }
        }
    }

}
