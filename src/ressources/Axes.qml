import QtQuick 2.7
import QtQuick.Layouts 1.1
import QmlPlotting 2.0 as QmlPlotting

Rectangle {
    id: root
    width: 400
    height: 300
    color: "transparent"

    property string xLabel: ""
    property string yLabel: ""
    property int tickXSpacing: 30
    property int tickYSpacing: 30
    property bool tickXLabels: true
    property bool tickYLabels: true
    property bool tickXGrid: true
    property bool tickYGrid: true
    property bool tickXMarker: true
    property bool tickYMarker: true
    property alias font: textMetric.font
    property color gridColor: "lightgray"
    property color textColor: "black"
    property int axesBorderWidth: 1
    property color axesBorderColor: "black"
    property color axesBackgroundColor: "transparent"

    property Item plotGroup: QmlPlotting.PlotGroup {
        Component.onCompleted: {
            plotGroup.parent = plotGroupItem;
            plotGroup.anchors.fill = plotGroupItem;
        }
    }

    onPlotGroupChanged: {
        // Reparent and anchor assigned plot group
        plotGroup.parent = plotGroupItem;
        plotGroup.anchors.fill = plotGroupItem;
    }

    TextMetrics {
        id: textMetric
        text: "-1000.00"
    }

    QtObject {
        id: xTicks
        property var model: calculateTicks(root.plotGroup.viewRect, plotGroupItem.width, root.tickXSpacing, textMetric.width)
        function calculateTicks(viewRect, width, spacing, text_width) {
            var x0 = viewRect.x;
            var xrange = viewRect.width;

            var max_ticks = Math.max(Math.floor(width / (text_width + spacing)) + 1, 2);
            var tickdiffprec = QmlPlotting.Utils.niceNumPrec(xrange / (max_ticks - 1), false);
            var tick_diff = tickdiffprec[0];
            var tick_prec = tickdiffprec[1];
            var tick_min = Math.ceil(x0 / tick_diff) * tick_diff;
            var n_ticks = Math.ceil((x0 + xrange - tick_min) / tick_diff);
            var tick_max = tick_min + n_ticks * tick_diff;

            var newTicks = [];
            for (var i=0; i < n_ticks; ++i) {
                var x = tick_min + tick_diff * i;
                var pos_x = (x - x0) * (width / xrange);
                if (pos_x > 0 && pos_x < width) {
                    var text = x.toFixed(tick_prec);
                    newTicks.push({pos: pos_x, text: text});
                }
            }
            return newTicks;
        }
    }

    QtObject {
        id: yTicks
        property var model: calculateTicks(root.plotGroup.viewRect, plotGroupItem.height, root.tickYSpacing, textMetric.height, root.plotGroup.logY)
        function calculateTicks(viewRect, height, spacing, text_height, logY) {
            var y0 = viewRect.y;
            var yrange = viewRect.height;

            var max_ticks = Math.max(Math.floor(height / (text_height + spacing)) + 1, 2);
            var tickdiffprec = QmlPlotting.Utils.niceNumPrec(yrange / (max_ticks - 1), logY);
            var tick_diff = tickdiffprec[0];
            var tick_prec = tickdiffprec[1];
            var tick_min = Math.floor(y0 / tick_diff) * tick_diff;
            var n_ticks = Math.ceil((y0 + yrange - tick_min) / tick_diff);
            var tick_max = tick_min + n_ticks * tick_diff;

            var newTicks = [];
            for (var i=0; i < n_ticks; ++i) {
                var y = tick_min + tick_diff * i;
                var pos_y = height - ((y - y0) * (height / yrange));
                if (pos_y > 0 && pos_y < height) {
                    var text = logY ? Math.pow(10, y).toExponential() : y.toFixed(tick_prec);
                    newTicks.push({pos: pos_y, text: text});
                }
            }
            return newTicks;
        }
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 10
        columns: 3
        rowSpacing: 5
        columnSpacing: 5

        // Y axis label
        Item {
            width: textMetric.height
            visible: yLabel

            Layout.row: 0
            Layout.column: 0
            Layout.fillHeight: true

            Text {
                anchors.centerIn: parent
                text: yLabel
                rotation: -90
                font: textMetric.font
                color: root.textColor
            }
        }

        // Y tick labels
        Item {
            width: textMetric.width
            visible: tickYLabels

            Layout.row: 0
            Layout.column: 1
            Layout.fillHeight: true

            Repeater {
                model: yTicks.model.length
                delegate: Text {
                    y: yTicks.model[index].pos - .5*height
                    text: yTicks.model[index].text
                    font: textMetric.font
                    color: root.textColor
                    width: textMetric.width
                    height: textMetric.height
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        Item {
            id: plotGroupItem
            Layout.row: 0
            Layout.column: 2
            Layout.fillHeight: true
            Layout.fillWidth: true

            // X grid lines
            Repeater {
                model: xTicks.model.length
                delegate: Rectangle {
                        visible: tickXGrid
                        color: root.gridColor
                        x: xTicks.model[index].pos
                        y: plotGroupItem.height - height
                        z: -1
                        width: 1
                        height: plotGroupItem.height
                }
            }
            // X tick markers
            Repeater {
                model: xTicks.model.length
                delegate: Rectangle {
                    visible: tickXMarker
                    color: root.textColor
                    x: xTicks.model[index].pos
                    y: plotGroupItem.height - height
                    z: 1
                    width: 1
                    height: 10
                }
            }
            // X grid lines
            Repeater {
                model: yTicks.model.length
                delegate: Rectangle {
                    visible: tickYGrid
                    color: root.gridColor
                    x: 0
                    y: yTicks.model[index].pos
                    z: -1
                    height: 1
                    width: plotGroupItem.width
                }
            }
            // Y tick markers
            Repeater {
                model: yTicks.model.length
                delegate: Rectangle {
                    visible: tickYMarker
                    color: root.textColor
                    x: 0
                    y: yTicks.model[index].pos
                    z: 1
                    height: 1
                    width: 10
                }
            }
            // Plot background
            Rectangle {
                anchors.fill: parent
                color: axesBackgroundColor
                z: -2
            }
            // Plot frame
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: axesBorderColor
                border.width: axesBorderWidth
                z: 1
            }
        }

        // X tick labels
        Item {
            height: textMetric.height
            visible: tickXLabels

            Layout.row: 1
            Layout.column: 2
            Layout.fillWidth: true

            Repeater {
                model: xTicks.model.length
                delegate: Text {
                    x: xTicks.model[index].pos - textMetric.width*.5
                    text: xTicks.model[index].text
                    font: textMetric.font
                    color: root.textColor
                    width: textMetric.width
                    height: textMetric.height
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                }
            }
        }

        // X axis label
        Item {
            height: textMetric.height
            visible: xLabel

            Layout.row: 2
            Layout.column: 2
            Layout.fillWidth: true

            Text {
                anchors.centerIn: parent
                text: xLabel
                font: textMetric.font
                color: root.textColor
            }
        }
    }
}
