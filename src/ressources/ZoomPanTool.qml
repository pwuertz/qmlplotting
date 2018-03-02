import QtQuick 2.7
import QtQuick.Controls 1.1
import QmlPlotting 2.0 as QmlPlotting

Item {
    id: root

    property Item plotGroup: parent

    signal pressed
    signal released

    // Mouse/Pinch area for zooming and panning viewRect
    PinchArea {
        enabled: root.enabled
        anchors.fill: parent
        onPinchStarted: root.pressed()
        onPinchFinished: root.released()
        onPinchUpdated: {
            // TODO: Check new coordinate transform on touch device
            var viewRect = root.plotGroup.viewRect;
            var newViewRect = Qt.rect(viewRect.x, viewRect.y, viewRect.width, viewRect.height);
            // Calculate pinch center move in plot space
            var delta = Qt.point((pinch.center.x - pinch.previousCenter.x) * viewRect.width / root.width,
                                 (pinch.previousCenter.y - pinch.center.y) * viewRect.height / root.height);
            var scale = 1. / (1. + pinch.scale - pinch.previousScale);

            // Apply pan
            newViewRect.x -= delta.x;
            newViewRect.y -= delta.y;
            // Apply zoom
            newViewRect.x = newViewRect.x + pinch.center.x / root.width * newViewRect.width * (1. - scale);
            newViewRect.y = newViewRect.y + (root.height - pinch.center.y) / root.height * newViewRect.height * (1. - scale);
            newViewRect.width *= scale;
            newViewRect.height *= scale;
            root.plotGroup.viewRect = newViewRect;
        }

        MouseArea {
            property point old_p: Qt.point(0, 0)
            property point pressed_p: Qt.point(0, 0)
            property bool zoom_mode: false
            acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MidButton
            enabled: root.enabled
            anchors.fill: parent

            onWheel: {
                var scale = 1. - wheel.angleDelta.y * (0.25 / 120.);
                var oldViewRect = root.plotGroup.viewRect;
                var newViewRect = Qt.rect(oldViewRect.x, oldViewRect.y, oldViewRect.width, oldViewRect.height);
                newViewRect.x = newViewRect.x + wheel.x/root.width * newViewRect.width * (1. - scale);
                newViewRect.y = newViewRect.y + (1. - wheel.y/root.height) * newViewRect.height * (1. - scale);
                newViewRect.width *= scale;
                newViewRect.height *= scale;
                root.plotGroup.viewRect = newViewRect;
            }
            onPressed: {
                // Store coordinates and set zoom/pan mode on mouse-down
                root.pressed()
                pressed_p = Qt.point(mouse.x, mouse.y);
                old_p = Qt.point(mouse.x, mouse.y);
                zoom_mode = pressedButtons & Qt.RightButton;
            }
            onPositionChanged: {
                var viewRect = root.plotGroup.viewRect;
                var newViewRect = Qt.rect(viewRect.x, viewRect.y, viewRect.width, viewRect.height);
                // Calculate mouse delta in plot space
                var delta = Qt.point((mouse.x - old_p.x) * viewRect.width / root.width,
                                     (old_p.y - mouse.y) * viewRect.height / root.height);
                old_p = Qt.point(mouse.x, mouse.y);
                if (zoom_mode) {
                    // Zoom mode, scale view at mouse down coordinates
                    var scalex, scaley;
                    if (!root.plotGroup.aspectAuto) {
                        var relDelta = Math.abs(delta.x) > Math.abs(delta.y) ? delta.x/viewRect.width : delta.y/viewRect.height;
                        scalex = 1. - 2. * relDelta;
                        scaley = 1. - 2. * relDelta;
                    } else {
                        scalex = 1. - 2. * delta.x / viewRect.width;
                        scaley = 1. - 2. * delta.y / viewRect.height;
                    }
                    var pressedCoords = Qt.point(pressed_p.x * viewRect.width / root.width,
                                                (root.height - pressed_p.y) * viewRect.height / root.height);
                    newViewRect.x = viewRect.x + pressedCoords.x * (1. - scalex);
                    newViewRect.y = viewRect.y + pressedCoords.y * (1. - scaley);
                    newViewRect.width *= scalex;
                    newViewRect.height *= scaley;
                } else {
                    // Pan mode, move view according to mouse delta
                    newViewRect.x -= delta.x;
                    newViewRect.y -= delta.y;
                }
                root.plotGroup.viewRect = newViewRect;
            }
            onReleased: {
                root.released()
            }
        }
    }
}
