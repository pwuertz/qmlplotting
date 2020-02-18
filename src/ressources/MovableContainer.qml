import QtQuick 2.7
import QmlPlotting 2.0 as QmlPlotting

QmlPlotting.Container {
    id: root
    property bool resizable: true
    property bool movable: true
    property bool aspectFixed: false
    property rect maxItemRect: Qt.rect(-Infinity, -Infinity, Infinity, Infinity)

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
                // Map new position to axes coordinates
                const pressedRect = frame.pressedItemRect;
                const pNow = mapToGlobal(mouse.x, mouse.y);
                let itemX = pressedRect.x + (pNow.x - frame.pressedCoords.x) * root.viewRect.width / root.width;
                let itemY = pressedRect.y + (frame.pressedCoords.y - pNow.y) * root.viewRect.height / root.height;
                // Restrict new position movement to max rect
                const maxRect = root.maxItemRect;
                if (maxRect.width !== Infinity) {
                    itemX = Math.min(maxRect.x + maxRect.width - pressedRect.width, Math.max(maxRect.x, itemX));
                }
                if (maxRect.height !== Infinity) {
                    itemY = Math.min(maxRect.y + maxRect.height - pressedRect.height, Math.max(maxRect.y, itemY));
                }
                // Apply
                root.itemRect = Qt.rect(itemX, itemY, pressedRect.width, pressedRect.height);
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

        // Top left handle
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
                    // Calculate delta from initial, pressed state
                    const pressedRect = frame.pressedItemRect;
                    const pNow = mapToGlobal(mouse.x, mouse.y);
                    const delta = Qt.point((pNow.x - frame.pressedCoords.x) * root.viewRect.width / root.width,
                                           (frame.pressedCoords.y - pNow.y) * root.viewRect.height / root.height);
                    // Restrict size reduction
                    const maxReduction = 0.9;
                    delta.x = Math.min(delta.x, pressedRect.width * maxReduction);
                    delta.y = Math.max(delta.y, -pressedRect.height * maxReduction);
                    // Restrict movement by aspect (TODO: implement non 1:1)
                    if (root.aspectFixed) {
                        const deltaMean = .5 * (delta.x - delta.y);
                        delta.x = deltaMean; delta.y = -deltaMean;
                    }
                    // Clamp deltas to max rect
                    const maxRect = root.maxItemRect;
                    delta.x = Math.max(maxRect.x, delta.x + pressedRect.x) - pressedRect.x;
                    if (root.aspectFixed) { delta.y = -delta.x; }
                    if (maxRect.height !== Infinity) {
                        delta.y = Math.min(maxRect.y + maxRect.height, pressedRect.y + pressedRect.height + delta.y) - (pressedRect.y + pressedRect.height);
                    }
                    if (root.aspectFixed) { delta.x = -delta.y; }
                    // Apply to rect position / size
                    root.itemRect = Qt.rect(
                                pressedRect.x + delta.x, pressedRect.y,
                                pressedRect.width - delta.x, pressedRect.height + delta.y);
                }
            }
        }
        // Bottom right handle (TODO: Use common resize handle definition)
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
                    // Calculate delta from initial, pressed state
                    const pressedRect = frame.pressedItemRect;
                    const pNow = mapToGlobal(mouse.x, mouse.y);
                    const delta = Qt.point((pNow.x - frame.pressedCoords.x) * root.viewRect.width / root.width,
                                           (frame.pressedCoords.y - pNow.y) * root.viewRect.height / root.height);
                    // Restrict size reduction
                    const maxReduction = 0.9;
                    delta.x = Math.max(delta.x, -pressedRect.width * maxReduction);
                    delta.y = Math.min(delta.y, pressedRect.height * maxReduction);
                    // Restrict movement by aspect (TODO: implement non 1:1)
                    if (root.aspectFixed) {
                        const deltaMean = .5 * (delta.x - delta.y);
                        delta.x = deltaMean; delta.y = -deltaMean;
                    }
                    // Clamp deltas to max rect
                    const maxRect = root.maxItemRect;
                    delta.y = Math.max(maxRect.y, delta.y + pressedRect.y) - pressedRect.y;
                    if (root.aspectFixed) { delta.x = -delta.y; }
                    if (maxRect.width !== Infinity) {
                        delta.x = Math.min(maxRect.x + maxRect.width, pressedRect.x + pressedRect.width + delta.x) - (pressedRect.x + pressedRect.width);
                    }
                    if (root.aspectFixed) { delta.y = -delta.x; }
                    // Apply to rect position / size
                    root.itemRect = Qt.rect(
                                pressedRect.x, pressedRect.y + delta.y,
                                pressedRect.width + delta.x, pressedRect.height - delta.y);
                }
            }
        }
    }
}
