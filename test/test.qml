import QtQuick 2.0
import QtQuick.Controls 1.1
import qmlplotting 1.0

Item {
    width: 640; height: 512

    function getItems() {
        return [source, image, selection_user]
    }

    DataSource {
        id: source
        Component.onCompleted: source.setTestData2D()
    }

    ZoomPanImage {
        id: image
        anchors.fill: parent
        dataSource: source
        minimumValue: min_slider.value
        maximumValue: max_slider.value
        colormap: "bwr"

        SelectionBox {
            id: selection_user
        }

        Rectangle {
            id: scanline
            property point p1: Qt.point(x / parent.width, y / parent.height)
            property point p2: Qt.point((x + Math.cos(rotation * Math.PI / 180.)*width) / parent.width,
                                        (y + Math.sin(rotation * Math.PI / 180.)*width) / parent.height)

            x: 156; y: 140
            width: 400
            height: 1.5
            opacity: .7
            color: "black"
            rotation: 30
            transformOrigin: Item.Left

            Rectangle {
                anchors.fill: scanline_text
                anchors.margins: -2
                border.width: 1
                border.color: "black"
                color: "#77ffffff"
                radius: 4

                MouseArea {
                    anchors.fill: parent
                    anchors.margins: -5
                    drag.target: scanline
                    drag.axis: Drag.XandYAxis
                    drag.threshold: 0
                }
            }

            Text {
                id: scanline_text
                y: - height - 3
                text: "Scan"
            }
        }
    }

    Row {
        x: 3; y: 3
        spacing: 3

        Button {
            text: "New Data"
            onClicked: source.setTestData2D()
        }

        ColormappedImage {
            width: 100; height: 100
            dataSource: source
            minimumValue: .5
            maximumValue: 1.
            colormap: image.colormap
            opacity: .75
            MouseArea {
                anchors.fill: parent
                drag.target: parent
                drag.axis: Drag.XandYAxis
            }
            Rectangle {
                color: "transparent"
                border.width: 1
                border.color: "black"
                radius: 4
                anchors.fill: parent
                anchors.margins: -1
            }
            SelectionBox {rect: selection_user.rect; movable: false}
        }

        ColormappedImage {
            width: 100; height: 100
            dataSource: source
            minimumValue: 0.
            maximumValue: .5
            colormap: image.colormap
            opacity: .75
            MouseArea {
                anchors.fill: parent
                drag.target: parent
                drag.axis: Drag.XandYAxis
            }
            Rectangle {
                color: "transparent"
                border.width: 1
                border.color: "black"
                radius: 4
                anchors.fill: parent
                anchors.margins: -1
            }
            SelectionBox {rect: selection_user.rect; movable: false}
        }

        Rectangle {
            id: slider_box
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
                    value: -.1
                    minimumValue: -.5
                    maximumValue: 1.5
                }
                Slider {
                    id: max_slider
                    width: 256
                    value: 1.1
                    minimumValue: -.5
                    maximumValue: 1.5
                }
            }
        }
    }

    Rectangle {
        id: plot_box
        x: 3; y: 370
        width: plots.width; height: plot_box_top.height + plots.height
        border.width: 1
        border.color: "black"
        color: "#77ffffff"
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
                text: "Linescan"
            }
            MouseArea {
                anchors.fill: parent
                drag.target: plot_box
                drag.axis: Drag.XandYAxis
            }
        }
        Item {
            y: plot_box_top.height
            width: 302
            height: 80
            id: plots

            SlicePlot {
                id: plot1
                anchors.fill: parent
                anchors.margins: 1
                dataSource: source
                color: "#99666666"
                numSegments: parent.width
                minimumValue: min_slider.value
                maximumValue: max_slider.value
                p1: scanline.p1
                p2: scanline.p2
                filled: true
            }
            SlicePlot {
                id: plot2
                anchors.fill: parent
                anchors.margins: 1
                dataSource: source
                color: "#ff666666"
                numSegments: parent.width
                minimumValue: min_slider.value
                maximumValue: max_slider.value
                p1: scanline.p1
                p2: scanline.p2
            }
        }
    }
}
