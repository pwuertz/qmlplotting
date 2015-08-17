import QtQuick 2.4
import QtQuick.Layouts 1.1
import qmlplotting 1.0

Rectangle {
    id: plotarea
    width: 400
    height: 300
    color: "transparent"

    property alias viewRect: zoom_pan_area.viewRect
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
    property alias tickFont: text_metric_tick.font
    property color gridColor: "lightgray"
    property color textColor: "black"
    property int axesBorderWidth: 1
    property color axesBorderColor: "black"
    property color axesBackgroundColor: "transparent"
    property bool logY: false

    TextMetrics {
        id: text_metric_tick
        text: "-1000.00"
    }

    QtObject {
        id: xticks
        property int spacing: plotarea.tickXSpacing
        property int precision: niceprecision(viewRect.width / (maxTicks - 1))
        property int maxTicks: Math.max(Math.floor(zoom_pan_area.width / (text_metric_tick.width + spacing)) + 1, 2)
        property int numTicks: Math.ceil((viewRect.x + viewRect.width - min) / tickDiff)
        property real tickDiff: nicenum(viewRect.width / (maxTicks - 1), false)
        property real min: Math.ceil(viewRect.x / tickDiff) * tickDiff
        property real max: min + numTicks * tickDiff
    }

    QtObject {
        id: yticks
        property real spacing: plotarea.tickYSpacing
        property int precision: niceprecision(viewRect.height / (maxTicks - 1))
        property int maxTicks: Math.max(Math.floor(zoom_pan_area.height / (text_metric_tick.height + spacing)) + 1, 2)
        property int numTicks: Math.ceil((viewRect.y + viewRect.height - min) / tickDiff)
        property real tickDiff: nicenum(viewRect.height / (maxTicks - 1), logY)
        property real min: Math.floor(viewRect.y / tickDiff) * tickDiff
        property real max: min + numTicks * tickDiff
        property var tickVals: new_tick_vals(min, max, tickDiff, numTicks, viewRect, zoom_pan_area.height)
        function new_tick_vals(min, max, diff, n, viewRect, height) {
            var newVals = []
            var newMajor = []
            var newGrid = []
            for (var i=0; i<n; ++i) {
                var val = min + diff * i
                var y = height - (val - viewRect.y) * height/viewRect.height
                if (y > 0 && y < height) {
                    newVals.push(val)
                    newMajor.push(y)
                    newGrid.push(y)
                }
                if (logY) {
                    for (var j=2; j<10; ++j) {
                        var val_minor = val + Math.log(j)/Math.LN10 * diff
                        var y_minor = height - (val_minor - viewRect.y) * height/viewRect.height
                        if ((y_minor > 0) && (y_minor < height)) {
                            newGrid.push(y_minor)
                        }
                    }
                }
            }
            tickPosMajor = newMajor
            tickPosGrid = newGrid
            return newVals
        }
        property var tickPosMajor
        property var tickPosGrid
    }

    function nicenum(range, pow10) {
        var exponent = Math.floor(Math.log(range) / Math.LN10);
        var fraction = range / Math.pow(10, exponent);
        var nicefrac;
        if (fraction <= 1)        nicefrac = 1;
        else if (fraction <= 2)   nicefrac = 2;
        else if (fraction <= 2.5) nicefrac = 2.5;
        else if (fraction <= 5)   nicefrac = 5;
        else                      nicefrac = 10;
        var result = nicefrac * Math.pow(10, exponent);
        if (pow10)
            return Math.ceil(Math.max(1, result))
        else
            return result
    }

    function niceprecision(range) {
        var exponent = Math.floor(Math.log(range) / Math.LN10);
        return Math.max(-exponent, 0)
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 10
        columns: 3
        rowSpacing: 5
        columnSpacing: 5

        Item {
            // ylabel
            width: text_metric_tick.height
            visible: yLabel

            Layout.row: 0
            Layout.column: 0
            Layout.fillHeight: true

            Text {
                anchors.centerIn: parent
                text: yLabel
                rotation: -90
                font: text_metric_tick.font
                color: plotarea.textColor
            }
        }

        Item {
            id: yticks_item
            width: text_metric_tick.width
            visible: tickYLabels

            Layout.row: 0
            Layout.column: 1
            Layout.fillHeight: true

            Repeater {
                model: Math.min(yticks.tickPosMajor.length, yticks.tickVals.length)
                delegate: Text {
                    y: yticks.tickPosMajor[index] - .5*height
                    property real tickVal: yticks.tickVals[index]
                    text: logY ? Math.pow(10, tickVal).toExponential() : tickVal.toFixed(yticks.precision)
                    font: text_metric_tick.font
                    color: plotarea.textColor
                    width: text_metric_tick.width
                    height: text_metric_tick.height
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        ZoomPanArea {
            id: zoom_pan_area
            Layout.row: 0
            Layout.column: 2
            Layout.fillHeight: true
            Layout.fillWidth: true

            // xtick grid and markers
            Repeater {
                model: xticks.numTicks
                delegate: Item {
                    property real tickVal: xticks.min + xticks.tickDiff * index
                    property real tickPos: (tickVal - viewRect.x) * zoom_pan_area.width/viewRect.width
                    Rectangle {
                        visible: tickXGrid
                        color: plotarea.gridColor
                        x: tickPos
                        y: zoom_pan_area.height - height
                        z: -1
                        width: 1
                        height: zoom_pan_area.height
                    }
                    Rectangle {
                        visible: tickXMarker
                        color: plotarea.textColor
                        x: tickPos
                        y: zoom_pan_area.height - height
                        z: 1
                        width: 1
                        height: 10
                    }
                }
            }

            // ytick grid and markers
            Repeater {
                model: yticks.tickPosGrid
                delegate: Item {
                    Rectangle {
                        visible: tickYGrid
                        color: plotarea.gridColor
                        x: 0
                        y: modelData
                        z: -1
                        height: 1
                        width: zoom_pan_area.width
                    }
                    Rectangle {
                        visible: tickYMarker
                        color: plotarea.textColor
                        x: 0
                        y: modelData
                        z: 1
                        height: 1
                        width: 10
                    }
                }
            }

            // frame
            Rectangle {
                anchors.fill: parent
                color: axesBackgroundColor
                z: -2
            }
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: axesBorderColor
                border.width: axesBorderWidth
                z: 1
            }
        }

        Item {
            id: xticks_item
            height: text_metric_tick.height
            visible: tickXLabels

            Layout.row: 1
            Layout.column: 2
            Layout.fillWidth: true

            Repeater {
                model: xticks.numTicks
                delegate: Text {
                    property real tickVal: xticks.min + xticks.tickDiff * index
                    x: (tickVal - viewRect.x) * zoom_pan_area.width/viewRect.width - text_metric_tick.width*.5
                    text: tickVal.toFixed(xticks.precision)
                    font: text_metric_tick.font
                    color: plotarea.textColor
                    width: text_metric_tick.width
                    height: text_metric_tick.height
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                }
            }
        }

        Item {
            // xlabel
            height: text_metric_tick.height
            visible: xLabel

            Layout.row: 2
            Layout.column: 2
            Layout.fillWidth: true

            Text {
                anchors.centerIn: parent
                text: xLabel
                font: text_metric_tick.font
                color: plotarea.textColor
            }
        }
    }

    property list<Item> plotItems

    Component.onCompleted: {
        this._updatePlotItems();
        onPlotItemsChanged.connect(this._updatePlotItems);
    }

    function _updatePlotItems() {
        for (var i = 0; i < plotItems.length; ++i) {
            plotItems[i].parent = zoom_pan_area;
            plotItems[i].anchors.fill = zoom_pan_area;
            plotItems[i].viewRect = Qt.binding(function() {return zoom_pan_area.viewRect})
            if ("logY" in plotItems[i]) {
                plotItems[i].logY = Qt.binding(function() {return plotarea.logY})
            }
        }
    }
}
