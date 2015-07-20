import QtQuick 2.0
import qmlplotting 1.0

Rectangle {
    id: plotarea
    width: 400
    height: 300
    color: "transparent"

    property alias viewRect: zoom_pan_area.viewRect
    property int tickXSpacing: 30
    property int tickYSpacing: 30
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

    Text {
        id: text_metric_tick
        visible: false
        text: "-1000.00"
    }

    QtObject {
        id: xticks
        property int spacing: plotarea.tickXSpacing
        property int precision: niceprecision(viewRect.width / (maxTicks - 1))
        property int maxTicks: Math.max(Math.floor(zoom_pan_area.width / (text_metric_tick.contentWidth + spacing)) + 1, 2)
        property int numTicks: Math.ceil((viewRect.x + viewRect.width - min) / tickDiff)
        property real tickDiff: nicenum(viewRect.width / (maxTicks - 1), false)
        property real min: Math.ceil(viewRect.x / tickDiff) * tickDiff
        property real max: min + numTicks * tickDiff
    }

    QtObject {
        id: yticks
        property real spacing: plotarea.tickYSpacing
        property int precision: niceprecision(viewRect.height / (maxTicks - 1))
        property int maxTicks: Math.max(Math.floor(zoom_pan_area.height / (text_metric_tick.contentHeight + spacing)) + 1, 2)
        property int numTicks: Math.ceil((viewRect.y + viewRect.height - min) / tickDiff)
        property real tickDiff: nicenum(viewRect.height / (maxTicks - 1), logY)
        property real min: Math.ceil(viewRect.y / tickDiff) * tickDiff
        property real max: min + numTicks * tickDiff
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
            return Math.max(1, result)
        else
            return result
    }

    function niceprecision(range) {
        var exponent = Math.floor(Math.log(range) / Math.LN10);
        return Math.max(-exponent, 0)
    }

    Grid {
        anchors.fill: parent
        anchors.margins: 10
        rows: 2
        columns: 2
        spacing: 6

        Item {
            id: yticks_item
            width: text_metric_tick.contentWidth
            height: zoom_pan_area.height
            Repeater {
                model: yticks.numTicks
                delegate: Text {
                    property real tickVal: yticks.min + yticks.tickDiff * index
                    y: zoom_pan_area.height - (tickVal - viewRect.y) * zoom_pan_area.height/viewRect.height - height*.5
                    text: logY ? Math.pow(10, tickVal).toExponential() : tickVal.toFixed(yticks.precision)
                    font: text_metric_tick.font
                    color: plotarea.textColor
                    width: text_metric_tick.contentWidth
                    height: text_metric_tick.contentHeight
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        ZoomPanArea {
            id: zoom_pan_area
            width: parent.width - yticks_item.width - parent.spacing
            height: parent.height - xticks_item.height - parent.spacing

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
                model: yticks.numTicks
                delegate: Item {
                    property real tickVal: yticks.min + yticks.tickDiff * index
                    property real tickPos: zoom_pan_area.height - (tickVal - viewRect.y) * zoom_pan_area.height/viewRect.height
                    Rectangle {
                        visible: tickYGrid
                        color: plotarea.gridColor
                        x: 0
                        y: tickPos
                        z: -1
                        height: 1
                        width: zoom_pan_area.width
                    }
                    Rectangle {
                        visible: tickXMarker
                        color: plotarea.textColor
                        x: 0
                        y: tickPos
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
            width: yticks_item.width
            height: xticks_item.height
        }

        Item {
            visible: true
            id: xticks_item
            height: text_metric_tick.contentHeight
            width: zoom_pan_area.width
            Repeater {
                model: xticks.numTicks
                delegate: Text {
                    property real tickVal: xticks.min + xticks.tickDiff * index
                    x: (tickVal - viewRect.x) * zoom_pan_area.width/viewRect.width - text_metric_tick.contentWidth*.5
                    text: tickVal.toFixed(xticks.precision)
                    font: text_metric_tick.font
                    color: plotarea.textColor
                    width: text_metric_tick.contentWidth
                    height: text_metric_tick.contentHeight
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                }
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
            plotItems[i].logY = Qt.binding(function() {return plotarea.logY})
        }
    }
}
