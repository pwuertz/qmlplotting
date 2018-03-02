import QtQuick 2.7
import QmlPlotting 2.0 as QmlPlotting

QmlPlotting.Container {
    id: root
    property bool resizable: true
    property bool movable: true
    property bool aspectFixed: false

    Item {
        id: frame
        visible: root.movable || root.resizable

        MouseArea {
            enabled: root.movable
            anchors.fill: parent
            cursorShape: Qt.OpenHandCursor

            onPressed: {
                frame.pressedCoords = mapToGlobal(mouse.x, mouse.y);
                frame.pressedItemRect = root.itemRect;
                cursorShape = Qt.ClosedHandCursor;
            }
            onReleased: { cursorShape = Qt.OpenHandCursor; }
            onPositionChanged: {
                var pNow = mapToGlobal(mouse.x, mouse.y);
                var delta = Qt.point((pNow.x - frame.pressedCoords.x) * root.viewRect.width / root.width,
                                     (frame.pressedCoords.y - pNow.y) * root.viewRect.height / root.height);
                root.itemRect = Qt.rect(frame.pressedItemRect.x + delta.x,
                                        frame.pressedItemRect.y + delta.y,
                                        frame.pressedItemRect.width,
                                        frame.pressedItemRect.height);
            }
        }

        Rectangle {
            anchors.fill: parent
            anchors.margins: -2
            color: "transparent"
            border.color: "gray"
            border.width: 2
        }

        property point pressedCoords
        property rect pressedItemRect

        // Top handle
        Rectangle {
            visible: root.resizable
            enabled: root.resizable
            x: -width * .5
            y: -height * .5
            width: 14
            height: 14
            radius: 7
            color: "lightgray"
            border.color: "black"
            border.width: 1
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.SizeFDiagCursor
                onPressed: {
                    frame.pressedCoords = mapToGlobal(mouse.x, mouse.y);
                    frame.pressedItemRect = root.itemRect;
                }
                onPositionChanged: {
                    var pNow = mapToGlobal(mouse.x, mouse.y);
                    var delta = Qt.point((pNow.x - frame.pressedCoords.x) * root.viewRect.width / root.width,
                                         (frame.pressedCoords.y - pNow.y) * root.viewRect.height / root.height);
                    if (root.aspectFixed) {
                        var deltaMean = .5 * (delta.x - delta.y);
                        delta = Qt.point(deltaMean, -deltaMean);
                    }
                    // Update item rect
                    var maxReduction = 0.9;
                    delta.x = Math.min(delta.x, frame.pressedItemRect.width * maxReduction);
                    delta.y = Math.max(delta.y, -frame.pressedItemRect.height * maxReduction);
                    root.itemRect = Qt.rect(frame.pressedItemRect.x + delta.x,
                                            frame.pressedItemRect.y,
                                            frame.pressedItemRect.width - delta.x,
                                            frame.pressedItemRect.height + delta.y);
                }
            }
        }
        // Bottom handle (TODO: Use common resize handle definition)
        Rectangle {
            visible: root.resizable
            enabled: root.resizable
            x: parent.width - width * .5
            y: parent.height - height * .5
            width: 14
            height: 14
            radius: 7
            color: "lightgray"
            border.color: "black"
            border.width: 1
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.SizeFDiagCursor
                onPressed: {
                    frame.pressedCoords = mapToGlobal(mouse.x, mouse.y);
                    frame.pressedItemRect = root.itemRect;
                }
                onPositionChanged: {
                    var pNow = mapToGlobal(mouse.x, mouse.y);
                    var delta = Qt.point((pNow.x - frame.pressedCoords.x) * root.viewRect.width / root.width,
                                         (frame.pressedCoords.y - pNow.y) * root.viewRect.height / root.height);
                    if (root.aspectFixed) {
                        var deltaMean = .5 * (delta.x - delta.y);
                        delta = Qt.point(deltaMean, -deltaMean);
                    }
                    // Update item rect
                    var maxReduction = 0.9;
                    delta.x = Math.max(delta.x, -frame.pressedItemRect.width * maxReduction);
                    delta.y = Math.min(delta.y, frame.pressedItemRect.height * maxReduction);
                    root.itemRect = Qt.rect(frame.pressedItemRect.x,
                                            frame.pressedItemRect.y + delta.y,
                                            frame.pressedItemRect.width + delta.x,
                                            frame.pressedItemRect.height - delta.y);
                }
            }
        }
    }
}
