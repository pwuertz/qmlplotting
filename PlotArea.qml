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
    property alias font: text_metric.font
    property color gridColor: "lightgray"
    property color textColor: "black"
    property int axesBorderWidth: 1
    property color axesBorderColor: "black"
    property color axesBackgroundColor: "transparent"
    property bool logY: false

    TextMetrics {
        id: text_metric
        text: "-1000.00"
    }

    QtObject {
        id: xticks
        property var tickModel: new_ticks(zoom_pan_area.viewRect, zoom_pan_area.width, plotarea.tickXSpacing, text_metric.width)
        function new_ticks(viewRect, width, spacing, text_width) {
            var x0 = viewRect.x;
            var xrange = viewRect.width;

            var max_ticks = Math.max(Math.floor(width / (text_width + spacing)) + 1, 2);
            var tickdiffprec = nice_num_prec(xrange / (max_ticks - 1), false);
            var tick_diff = tickdiffprec[0];
            var tick_prec = tickdiffprec[1];
            var tick_min = Math.ceil(x0 / tick_diff) * tick_diff;
            var n_ticks = Math.ceil((x0 + xrange - tick_min) / tick_diff);
            var tick_max = tick_min + n_ticks * tick_diff;

            var new_model = [];
            for (var i=0; i<n_ticks; ++i) {
                var x = tick_min + tick_diff * i;
                var pos_x = (x-x0) * (width/xrange);
                if (pos_x > 0 && pos_x < width) {
                    var text = x.toFixed(tick_prec);
                    new_model.push({pos: pos_x, text: text});
                }
            }
            return new_model;
        }
    }

    QtObject {
        id: yticks
        property var tickModel: new_ticks(zoom_pan_area.viewRect, zoom_pan_area.height, plotarea.tickYSpacing, text_metric.height)
        function new_ticks(viewRect, height, spacing, text_height) {
            var y0 = viewRect.y;
            var yrange = viewRect.height;

            var max_ticks = Math.max(Math.floor(height / (text_height + spacing)) + 1, 2);
            var tickdiffprec = nice_num_prec(yrange / (max_ticks - 1), logY);
            var tick_diff = tickdiffprec[0];
            var tick_prec = tickdiffprec[1];
            var tick_min = Math.floor(y0 / tick_diff) * tick_diff;
            var n_ticks = Math.ceil((y0 + yrange - tick_min) / tick_diff);
            var tick_max = tick_min + n_ticks * tick_diff;

            var new_model = [];
            for (var i=0; i<n_ticks; ++i) {
                var y = tick_min + tick_diff * i;
                var pos_y = height - ((y-y0) * (height/yrange));
                if (pos_y > 0 && pos_y < height) {
                    var text = logY ? Math.pow(10, y).toExponential() : y.toFixed(tick_prec);
                    new_model.push({pos: pos_y, text: text});
                }
            }
            return new_model;
        }
    }

    function nice_num_prec(range, pow10) {
        var exponent = Math.floor(Math.log(range) / Math.LN10);
        var fraction = range / Math.pow(10, exponent);
        var niceprec = -exponent;
        var nicefrac;
        if (fraction <= 1)        nicefrac = 1;
        else if (fraction <= 2)   nicefrac = 2;
        else if (fraction <= 2.5) {nicefrac = 2.5; niceprec += 1}
        else if (fraction <= 5)   nicefrac = 5;
        else                      {nicefrac = 10; niceprec -= 1}
        var nicediff = nicefrac * Math.pow(10, exponent);
        if (pow10)
            return [Math.ceil(Math.max(1, nicediff)), Math.max(niceprec, 0)]
        else
            return [nicediff, Math.max(niceprec, 0)]
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 10
        columns: 3
        rowSpacing: 5
        columnSpacing: 5

        Item {
            // ylabel
            width: text_metric.height
            visible: yLabel

            Layout.row: 0
            Layout.column: 0
            Layout.fillHeight: true

            Text {
                anchors.centerIn: parent
                text: yLabel
                rotation: -90
                font: text_metric.font
                color: plotarea.textColor
            }
        }

        Item {
            id: yticks_item
            width: text_metric.width
            visible: tickYLabels

            Layout.row: 0
            Layout.column: 1
            Layout.fillHeight: true

            // ytick labels
            Repeater {
                model: yticks.tickModel.length
                delegate: Text {
                    y: yticks.tickModel[index].pos - .5*height
                    text: yticks.tickModel[index].text
                    font: text_metric.font
                    color: plotarea.textColor
                    width: text_metric.width
                    height: text_metric.height
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

            // xtick grid
            Repeater {
                model: xticks.tickModel.length
                delegate: Rectangle {
                        visible: tickXGrid
                        color: plotarea.gridColor
                        x: xticks.tickModel[index].pos
                        y: zoom_pan_area.height - height
                        z: -1
                        width: 1
                        height: zoom_pan_area.height
                }
            }
            // xtick marker
            Repeater {
                model: xticks.tickModel.length
                delegate: Rectangle {
                    visible: tickXMarker
                    color: plotarea.textColor
                    x: xticks.tickModel[index].pos
                    y: zoom_pan_area.height - height
                    z: 1
                    width: 1
                    height: 10
                }
            }

            // ytick grid
            Repeater {
                model: yticks.tickModel.length
                delegate: Rectangle {
                    visible: tickYGrid
                    color: plotarea.gridColor
                    x: 0
                    y: yticks.tickModel[index].pos
                    z: -1
                    height: 1
                    width: zoom_pan_area.width
                }
            }

            // ytick marker
            Repeater {
                model: yticks.tickModel.length
                delegate: Rectangle {
                    visible: tickYMarker
                    color: plotarea.textColor
                    x: 0
                    y: yticks.tickModel[index].pos
                    z: 1
                    height: 1
                    width: 10
                }
            }

            // background
            Rectangle {
                anchors.fill: parent
                color: axesBackgroundColor
                z: -2
            }

            // frame
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
            height: text_metric.height
            visible: tickXLabels

            Layout.row: 1
            Layout.column: 2
            Layout.fillWidth: true

            // xtick labels
            Repeater {
                model: xticks.tickModel.length
                delegate: Text {
                    x: xticks.tickModel[index].pos - text_metric.width*.5
                    text: xticks.tickModel[index].text
                    font: text_metric.font
                    color: plotarea.textColor
                    width: text_metric.width
                    height: text_metric.height
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                }
            }
        }

        Item {
            // xlabel
            height: text_metric.height
            visible: xLabel

            Layout.row: 2
            Layout.column: 2
            Layout.fillWidth: true

            Text {
                anchors.centerIn: parent
                text: xLabel
                font: text_metric.font
                color: plotarea.textColor
            }
        }
    }

    property list<Item> plotItems

    property Instantiator instantiator: Instantiator{
        model: plotItems

        delegate: QtObject{ // dummy for connection handling
            Component.onCompleted: connectItem(model)
            Component.onDestruction: disconnectItem(model)
        }
    }
    function connectItem(item){
        item.parent = zoom_pan_area
        item.anchors.fill = zoom_pan_area
        // item.viewRect = Qt.binding(function() {return zoom_pan_area.viewRect})
        if("logY" in model){
            item.logY = Qt.binding(function() {return plotarea.logY})
        }
    }
    function disconnectItem(item){
        if("logY" in model){
            item.logY = false
        }
    }
}
