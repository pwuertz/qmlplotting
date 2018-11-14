import QtQuick 2.7
import QtQuick.Controls 1.1
import QtQuick.Shapes 1.0
import QmlPlotting 2.0 as QmlPlotting

Item {
    id: root
    property rect viewRect
    property alias polygon: polygonItem
    property alias points: polygonItem.points

    // Signal emitted when polygon editing finished
    signal polygonFinished(var points)

    // Reset and reactivate drawing tool
    function reset() {
        enabled = true;
        polygonItem.closed = false;
        polygonItem.points = [];
    }

    // Finish current polygon and deactivate drawing tool
    function finishPolygon() {
        enabled = false;
        polygonItem.closed = true;
        polygonItem.points = polygonItem.points;
        polygonFinished(polygonItem.points);
    }

    // Remove last point from current polygon
    function removeLastPoint() {
        polygonItem.points.pop();
        polygonItem.points = polygonItem.points;
    }

    // Mouse handlers for polygon drawing tool
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onPressed: {
            root.removeLastPoint();
        }
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent

        acceptedButtons: Qt.LeftButton
        hoverEnabled: true
        cursorShape: Qt.CrossCursor

        readonly property real xScale: root.viewRect.width / width
        readonly property real yScale: root.viewRect.height / height

        onPressed: {
            // Add new point on click
            var p = Qt.point(mouse.x * xScale + root.viewRect.x,
                             (height - mouse.y) * yScale + root.viewRect.y);
            polygonItem.points.push(p);
            polygonItem.points = polygonItem.points;
        }
        onPositionChanged: {
            // Preview polygon with new point at current mouse position
            newPointHint.newPoint = Qt.point(mouse.x, mouse.y);
        }
        onDoubleClicked: {
            // Close polygon and deactivate tool on double click
            root.finishPolygon();
        }
    }

    // Current polygon
    PolygonItem {
        id: polygonItem
        anchors.fill: parent
        viewRect: parent.viewRect
        closed: false
    }

    // Preview new point to be added to polygon
    Shape {
        id: newPointHint
        anchors.fill: parent
        visible: enabled && mouseArea.containsMouse && points.length

        property point p1: points.length ? points[0] : Qt.point(0, 0)
        property point p2: points.length ? points[points.length-1] : Qt.point(0, 0)
        property point newPoint

        readonly property real xScale: width / root.viewRect.width
        readonly property real yScale: height / root.viewRect.height
        readonly property real yOffset: (root.viewRect.height + root.viewRect.y) * yScale

        ShapePath {
            strokeWidth: polygonItem.strokeWidth
            strokeColor: polygonItem.strokeColor
            strokeStyle: polygonItem.strokeStyle
            fillColor: Qt.rgba(polygonItem.fillColor.r, polygonItem.fillColor.g, polygonItem.fillColor.b,
                               polygonItem.fillColor.a * 0.5)

            startX: (newPointHint.p2.x - root.viewRect.x) * newPointHint.xScale
            startY: newPointHint.yOffset - newPointHint.p2.y * newPointHint.yScale
            PathLine {
                x: newPointHint.newPoint.x
                y: newPointHint.newPoint.y
            }
            PathLine {
                x: (newPointHint.p1.x - root.viewRect.x) * newPointHint.xScale
                y: newPointHint.yOffset - newPointHint.p1.y * newPointHint.yScale
            }
        }
    }
}
