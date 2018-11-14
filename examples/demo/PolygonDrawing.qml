import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2
import QmlPlotting 2.0 as QmlPlotting
import ".."

Page {
    id: root
    QmlPlotting.Axes {
        anchors.fill: parent
        plotGroup: QmlPlotting.PlotGroup {
            clip: true
            viewRect: Qt.rect(-1, -1, 3, 3)
            aspectAuto: false
            plotItems: [
                QmlPlotting.ZoomPanTool {},
                PolygonTool {
                    polygon.strokeColor: "steelblue"
                    polygon.fillColor: "lightsteelblue"
                }
            ]
        }
    }
}
