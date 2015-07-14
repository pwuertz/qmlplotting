import QtQuick 2.0
import QtQuick.Layouts 1.1
import qmlplotting 1.0

Item {
    id: plotarea
    width: 400
    height: 300

    property alias viewRect: zoom_pan_area.viewRect
    property int tickXSpacing: 30
    property int tickYSpacing: 30
    property int tickPrecision: 2
    property alias tickFont: text_metric_tick.font

    Text {
        id: text_metric_tick
        visible: false
        text: "-1000.00"
    }

    QtObject {
        id: xticks
        property int spacing: plotarea.tickXSpacing
        property int precision: plotarea.tickPrecision
        property int maxTicks: Math.max(Math.floor(zoom_pan_area.width / (text_metric_tick.contentWidth + spacing)) + 1, 2)
        property int numTicks: Math.ceil((viewRect.x + viewRect.width - min) / tickDiff)
        property real tickDiff: nicenum(viewRect.width / (maxTicks - 1))
        property real min: Math.ceil(viewRect.x / tickDiff) * tickDiff
        property real max: min + numTicks * tickDiff
    }

    QtObject {
        id: yticks
        property real spacing: plotarea.tickYSpacing
        property int precision: plotarea.tickPrecision
        property int maxTicks: Math.max(Math.floor(zoom_pan_area.height / (text_metric_tick.contentHeight + spacing)) + 1, 2)
        property int numTicks: Math.ceil((viewRect.y + viewRect.height - min) / tickDiff)
        property real tickDiff: nicenum(viewRect.height / (maxTicks - 1))
        property real min: Math.ceil(viewRect.y / tickDiff) * tickDiff
        property real max: min + numTicks * tickDiff
    }

    function nicenum(range) {
        var exponent = Math.floor(Math.log(range) / Math.LN10);
        var fraction = range / Math.pow(10, exponent);
        var nicefrac;
        if (fraction <= 1)        nicefrac = 1;
        else if (fraction <= 2)   nicefrac = 2;
        else if (fraction <= 2.5) nicefrac = 2.5;
        else if (fraction <= 5)   nicefrac = 5;
        else                      nicefrac = 10;
        return nicefrac * Math.pow(10, exponent);
    }

    ZoomPanArea {
        id: zoom_pan_area
        anchors.fill: parent

        Repeater {
            model: xticks.numTicks
            delegate: Rectangle {
                color: "lightgray"
                property real tickVal: xticks.min + xticks.tickDiff * index
                x: (tickVal - viewRect.x) * zoom_pan_area.width/viewRect.width
                y: zoom_pan_area.height - height
                width: 1
                height: zoom_pan_area.height
            }
        }
        Repeater {
            model: yticks.numTicks
            delegate: Rectangle {
                color: "lightgray"
                property real tickVal: yticks.min + yticks.tickDiff * index
                x: 0
                y: zoom_pan_area.height - (tickVal - viewRect.y) * zoom_pan_area.height/viewRect.height
                height: 1
                width: zoom_pan_area.width
            }
        }
    }

    Item {
        height: text_metric_tick.contentHeight
        width: zoom_pan_area.width
        anchors.top: zoom_pan_area.bottom
        anchors.margins: 5
        Repeater {
            model: xticks.numTicks
            delegate: Text {
                property real tickVal: xticks.min + xticks.tickDiff * index
                x: (tickVal - viewRect.x) * zoom_pan_area.width/viewRect.width - text_metric_tick.contentWidth*.5
                text: tickVal.toFixed(xticks.precision)
                font: text_metric_tick.font
                width: text_metric_tick.contentWidth
                height: text_metric_tick.contentHeight
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop
            }
        }
    }

    Item {
        width: text_metric_tick.contentWidth
        height: zoom_pan_area.height
        anchors.right: zoom_pan_area.left
        anchors.margins: 8
        Repeater {
            model: yticks.numTicks
            delegate: Text {
                property real tickVal: yticks.min + yticks.tickDiff * index
                y: zoom_pan_area.height - (tickVal - viewRect.y) * zoom_pan_area.height/viewRect.height - height*.5
                text: tickVal.toFixed(yticks.precision)
                font: text_metric_tick.font
                width: text_metric_tick.contentWidth
                height: text_metric_tick.contentHeight
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
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
        }
    }
}
