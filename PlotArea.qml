import QtQuick 2.0
import QtQuick.Layouts 1.1
import qmlplotting 1.0

Item {
    width: 400
    height: 300

    property alias viewRect: zoom_pan_area.viewRect

    QtObject {
        id: xticks
        property real spacing: 10
        property int tickWidth: 100
        property int tickHeight: 20
        property int precision: 2
        property int numTicks: Math.max(Math.floor(zoom_pan_area.width / (tickWidth + spacing)) + 1, 2)
        property real tickDiff: nicenum(viewRect.width / (numTicks - 1))
        property real min: Math.floor(viewRect.x / tickDiff) * tickDiff
        property real max: Math.ceil((viewRect.x + viewRect.width) / tickDiff) * tickDiff
    }

    QtObject {
        id: yticks
        property real spacing: 40
        property int tickWidth: 50
        property int tickHeight: 20
        property int precision: 2
        property int numTicks: Math.max(Math.floor(zoom_pan_area.height / (tickHeight + spacing)) + 1, 2)
        property real tickDiff: nicenum(viewRect.height / (numTicks - 1))
        property real min: Math.floor(viewRect.y / tickDiff) * tickDiff
        property real max: Math.ceil((viewRect.y + viewRect.height) / tickDiff) * tickDiff
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
        clip: true

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

    Rectangle {
        color: "transparent"
        clip: true
        height: xticks.tickHeight
        width: zoom_pan_area.width
        anchors.top: zoom_pan_area.bottom
        anchors.margins: 5
        Repeater {
            model: xticks.numTicks
            delegate: Text {
                property real tickVal: xticks.min + xticks.tickDiff * index
                x: (tickVal - viewRect.x) * zoom_pan_area.width/viewRect.width - xticks.tickWidth*.5
                text: tickVal.toFixed(xticks.precision)
                width: xticks.tickWidth
                height: xticks.tickHeight
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop
            }
        }
    }

    Rectangle {
        color: "transparent"
        clip: true
        width: yticks.tickWidth
        height: zoom_pan_area.height
        anchors.right: zoom_pan_area.left
        anchors.margins: 8
        Repeater {
            model: yticks.numTicks
            delegate: Text {
                property real tickVal: yticks.min + yticks.tickDiff * index
                y: zoom_pan_area.height - (tickVal - viewRect.y) * zoom_pan_area.height/viewRect.height - yticks.tickHeight*.5
                text: tickVal.toFixed(yticks.precision)
                width: yticks.tickWidth
                height: yticks.tickHeight
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
