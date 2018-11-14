import QtQuick 2.7
import QtQml 2.2
import QtQuick.Shapes 1.0

Item {
    id: root

    property rect viewRect

    property var points: []
    property bool closed: true

    property int strokeWidth: 2
    property color strokeColor: "black"
    property int strokeStyle: ShapePath.SolidLine
    property color fillColor: "white"

    Shape {
        anchors.fill: parent
        id: shape
        readonly property real xScale: width / root.viewRect.width
        readonly property real yScale: height / root.viewRect.height
        readonly property real yOffset: (root.viewRect.height + root.viewRect.y) * yScale

        // Create PathLine to point with coordinate bindings
        function createPathLine(point, parent) {
            var line = pathLineComponent.createObject(parent);
            line.x = Qt.binding(function() {
                return (point.x - root.viewRect.x) * shape.xScale;
            });
            line.y = Qt.binding(function() {
                return shape.yOffset - point.y * shape.yScale;
            });
            return line;
        }

        // Create list of PathLine for given points
        function createPathLines(parent) {
            var lines = [];
            for (var i=1; i < root.points.length; ++i) {
                lines.push(createPathLine(root.points[i], parent));
            }
            if (root.closed) {
                lines.push(createPathLine(root.points[0], parent));
            }
            return lines;
        }

        Component {
            id: shapePathComponent
            ShapePath {
                strokeWidth: root.strokeWidth
                strokeColor: root.strokeColor
                strokeStyle: root.strokeStyle
                fillColor: root.fillColor

                startX: (points[0].x - root.viewRect.x) * shape.xScale
                startY: shape.yOffset - points[0].y * shape.yScale
                pathElements: shape.createPathLines(this)
            }
        }

        Component {
            id: pathLineComponent
            PathLine {}
        }

        Instantiator {
            model: root.points.length >= 2 ? [root.points] : []
            delegate: shapePathComponent
            onObjectAdded: { shape.data.push(object); }
            onObjectRemoved: { shape.data = []; }
        }
    }
}
