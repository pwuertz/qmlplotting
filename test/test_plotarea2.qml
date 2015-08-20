import QtQuick 2.0
import qmlplotting 1.0

Rectangle {
    width: 900; height: 500
    color: "gray"

    DataSource {
        id: source_img
        objectName: "source_img"
        Component.onCompleted: setTestData2D()
    }

    Window {
        title: "Plot Area"
        objectName: "plotwindow"
        x: 10
        y: 10

        PlotArea {
            id: plotarea
            objectName: "plotarea"
            width: 800
            height: 400

            axesBackgroundColor: "transparent"
            axesBorderColor: "#33000000"
            axesBorderWidth: 4

            plotItems: [
                ColormappedImage {
                    id: image
                    objectName: "image"
                    dataSource: source_img
                    colormap: "viridis"
                    viewInvert: true
                }
            ]
        }
    }
}
