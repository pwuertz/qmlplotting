import QtQuick 2.3
import QmlPlotting 1.0 as QmlPlotting
import QtTest 1.0

QmlPlotting.PlotArea {
    id: plotArea
    width: 512
    height: 512

    plotItems: [
        QmlPlotting.ColormappedImage {
            id: colormappedImage
            anchors.fill: parent
            dataSource: QmlPlotting.DataSource {}
        },
        QmlPlotting.XYPlot {
            id: xyPlot
            anchors.fill: parent
            dataSource: QmlPlotting.DataSource {}
        }
    ]

    TestCase {
        name: "XYPlot"

        // Test loading example data
        function test_exampledata() {
            xyPlot.dataSource.setTestData1D();
        }
    }

    TestCase {
        name: "ColormappedImage"

        // Test loading example data
        function test_exampledata() {
            colormappedImage.dataSource.setTestData2D();
        }
    }

    TestCase {
        name: "ZoomPanArea"

        // Test viewRect binding of plotItems
        function test_viewRect() {
            plotArea.viewRect = Qt.rect(0, 0, .5, .5);
            compare(plotArea.viewRect, xyPlot.viewRect);
            compare(plotArea.viewRect, colormappedImage.viewRect);
        }
    }
}
