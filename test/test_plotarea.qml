import QtQuick 2.0
import qmlplotting 1.0

Rectangle {
    width: 900; height: 500
    color: "#eee"

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

    PlotArea {
        id: plotarea
        objectName: "plotarea"
        anchors.fill: parent

        axesBackgroundColor: "transparent"
        axesBorderColor: "#33000000"
        axesBorderWidth: 4
        xLabel: "X Axis"
        yLabel: "Y Axis"

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
