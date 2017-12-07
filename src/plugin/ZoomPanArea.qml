import QtQuick 2.0
import QtQuick.Controls 1.1
import QmlPlotting 1.0

Item {
    id: root
    property bool movable: true
    property bool fixaspect: false
    property rect viewRect: Qt.rect(0., 0., 1., 1.)

    signal pressed
    signal released

    // mouse and pinch area for zoom/pan viewRect
    PinchArea {
        enabled: root.movable
        anchors.fill: parent
        onPinchStarted: root.pressed()
        onPinchFinished: root.released()
        onPinchUpdated: {
            var dx = pinch.center.x - pinch.previousCenter.x;
            var dy = pinch.center.y - pinch.previousCenter.y;
            var scale = 1. / (1. + pinch.scale - pinch.previousScale);

            var newViewRect = Qt.rect(viewRect.x, viewRect.y, viewRect.width, viewRect.height);
            // apply pan
            newViewRect.x -= dx * (newViewRect.width/root.width);
            newViewRect.y += dy * (newViewRect.height/root.height);
            // apply zoom
            newViewRect.x = newViewRect.x + pinch.center.x/root.width * newViewRect.width * (1. - scale);
            newViewRect.y = newViewRect.y + (1. - pinch.center.y/root.height) * newViewRect.height * (1. - scale);
            newViewRect.width *= scale;
            newViewRect.height *= scale;

            viewRect = newViewRect;
        }

        MouseArea {
            property point old_p: Qt.point(0, 0)
            property point pressed_p: Qt.point(0, 0)
            property bool zoom_mode: false
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            enabled: root.movable
            anchors.fill: parent

            onWheel: {
                var scale = 1. - wheel.angleDelta.y * (0.25 / 120.);
                var newViewRect = Qt.rect(viewRect.x, viewRect.y, viewRect.width, viewRect.height);
                newViewRect.x = newViewRect.x + wheel.x/root.width * newViewRect.width * (1. - scale);
                newViewRect.y = newViewRect.y + (1. - wheel.y/root.height) * newViewRect.height * (1. - scale);
                newViewRect.width *= scale;
                newViewRect.height *= scale;
                viewRect = newViewRect;
            }
            onPressed: {
                root.pressed()
                pressed_p = Qt.point(mouse.x, mouse.y);
                old_p = Qt.point(mouse.x, mouse.y);
                zoom_mode = pressedButtons & Qt.RightButton;
            }
            onPositionChanged: {
                var delta = Qt.point(mouse.x - old_p.x, mouse.y - old_p.y);
                old_p = Qt.point(mouse.x, mouse.y);
                var newViewRect = Qt.rect(viewRect.x, viewRect.y, viewRect.width, viewRect.height);
                if (zoom_mode) {
                    // zoom mode
                    var scalex, scaley;
                    if (root.fixaspect) {
                        var d = Math.abs(delta.x) > Math.abs(delta.y) ? -delta.x/root.width : delta.y/root.height;
                        scalex = 1. + 2. * d;
                        scaley = 1. + 2. * d;
                    } else {
                        scalex = 1. - 2. * delta.x/root.width;
                        scaley = 1. + 2. * delta.y/root.height;
                    }
                    newViewRect.x = newViewRect.x + pressed_p.x/root.width * newViewRect.width * (1. - scalex);
                    newViewRect.y = newViewRect.y + (1. - pressed_p.y/root.height) * newViewRect.height * (1. - scaley);
                    newViewRect.width *= scalex;
                    newViewRect.height *= scaley;
                } else {
                    // pan mode
                    newViewRect.x -= delta.x * (newViewRect.width/root.width);
                    newViewRect.y += delta.y * (newViewRect.height/root.height);
                }
                viewRect = newViewRect;
            }
            onReleased: {
                root.released()
            }
        }
    }
}
