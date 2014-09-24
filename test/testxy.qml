import QtQuick 2.0
import QtQuick.Controls 1.1
import qmlplotting 1.0

Rectangle {
    width: 640; height: 512
    color: "gray"

    function getItems() {
        return [source, xyplot]
    }

    DataSource {
        id: source
        Component.onCompleted: source.setTestData1D()
    }

    Button {
        text: "New Data"
        onClicked: source.setTestData1D()
    }

    Rectangle {
        id: plot_box
        x: 5; y: 35
        width: xyplot.width + 2; height: xyplot.height + 2 + plot_box_top.height
        border.width: 1
        border.color: "black"
        color: "#99ffffff"
        radius: 4

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

        XYPlot {
            id: xyplot
            width: 630; height: 200
            y: plot_box_top.height
            x: 1
            dataSource: source
            xMin: min_slider.value
            xMax: max_slider.value
            yMax: 1.1
            markerColor: "#3333cc"
            markerSize: 10
            opacity: .5
        }
    }

    Rectangle {
        id: slider_box
        x: 10
        y: 270
        width: sliders.width + 10; height: sliders.height + 10 + slider_box_top.height
        border.width: 1
        border.color: "black"
        color: "#77ffffff"
        radius: 4

        Rectangle {
            id: slider_box_top
            width: parent.width; height: slider_box_label.height + 4
            border.width: 1
            border.color: "black"
            color: "#77ffffff"
            radius: 4
            Text {
                x: 4; y: 2
                id: slider_box_label
                text: "Minimum / Maximum"
            }
            MouseArea {
                anchors.fill: parent
                drag.target: slider_box
                drag.axis: Drag.XandYAxis
            }
        }
        Column {
            x: 5; y: slider_box_top.height + 5
            id: sliders
            spacing: 5
            Slider {
                id: min_slider
                width: 256
                value: -1.1
                minimumValue: -1.1
                maximumValue: 1.1
            }
            Slider {
                id: max_slider
                width: 256
                value: 1.1
                minimumValue: -1.1
                maximumValue: 1.1
            }
        }
    }
}
