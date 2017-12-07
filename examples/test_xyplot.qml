import QtQuick 2.0
import QtQml 2.2
import QtQuick.Controls 1.1
import QmlPlotting 1.0

Rectangle {
    width: 640; height: 302
    color: "gray"

    DataSource {
        id: source
        objectName: "source"
        Component.onCompleted: source.setTestData1D()
    }

    Button {
        text: "New Data"
        objectName: "button"
        onClicked: source.setTestData1D()
    }

    Window {
        x: 5
        y: 35
        title: "XY Plot"

        XYPlot {
            id: xyplot
            objectName: "xyplot"
            width: 630; height: 200
            dataSource: source
            viewRect: Qt.rect(min_slider.value, 0., max_slider.value-min_slider.value, 1.1)
            fillEnabled: true
            fillColor: "#44aaaaff"
            lineEnabled: true
            lineWidth: 4.
            lineColor: "#8888cc"
            markerEnabled: true
            markerColor: "#998888cc"
            markerSize: 13
            markerSegments: 3
            markerBorder: true
        }
    }

    Window {
        x: 350
        y: 200
        title: "Plot Range"
        color: "#cceeeeee"
        flat: true

        Column {
            id: sliders
            spacing: 5
            Slider {
                id: min_slider
                width: 256
                value: -1.
                minimumValue: -1.1
                maximumValue: 1.1
            }
            Slider {
                id: max_slider
                width: 256
                value: 1.
                minimumValue: -1.1
                maximumValue: 1.1
            }
        }
    }
}
