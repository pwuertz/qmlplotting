import QtQuick 2.7
import QtTest 1.0
import QmlPlotting 2.0 as QmlPlotting

QmlPlotting.PlotGroup {
    id: plotGroup
    width: 512
    height: 512

    // Add items under test to plot group
    plotItems: [
        QmlPlotting.ColormappedImage {
            id: colormappedImage
            dataSource: QmlPlotting.DataSource {}
        },
        QmlPlotting.XYPlot {
            id: xyPlot
            dataSource: QmlPlotting.DataSource {}
        }
    ]

    TestCase {
        name: "XYPlot"
        function test_setTestData() {
            xyPlot.dataSource.setTestData1D();
        }
    }

    TestCase {
        name: "ColormappedImage"
        function test_setTestData() {
            colormappedImage.dataSource.setTestData2D();
        }
    }

    TestCase {
        name: "PlotGroup"
        function test_viewRectBinding() {
            plotGroup.viewRect = Qt.rect(-1, -1, 2, 2);
            compare(plotGroup.viewRect, xyPlot.viewRect);
            compare(plotGroup.viewRect, colormappedImage.viewRect);
        }
    }
}
