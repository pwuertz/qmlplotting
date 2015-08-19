import QtQuick 2.0
import QtQuick.Controls 1.1
import qmlplotting 1.0

Item {
    property bool movable: true
    property rect viewRect: Qt.rect(0., 0., 1., 1.)

    // mouse area for zooming/panning the view
    MouseArea {
        property point old_p: Qt.point(0, 0)
        property point pressed_p: Qt.point(0, 0)
        property bool zoom_mode: false
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        anchors.fill: parent
        enabled: parent.movable

        onWheel: {
            var scale = 1. - wheel.angleDelta.y * (0.25 / 120.);
            var newViewRect = Qt.rect(viewRect.x, viewRect.y, viewRect.width, viewRect.height);
            newViewRect.x = newViewRect.x + wheel.x/parent.width * newViewRect.width * (1. - scale);
            newViewRect.y = newViewRect.y + (1. - wheel.y/parent.height) * newViewRect.height * (1. - scale);
            newViewRect.width *= scale;
            newViewRect.height *= scale;
            viewRect = newViewRect;
        }
        onPressed: {
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
                var scalex = 1. - 2. * delta.x/parent.width;
                var scaley = 1. + 2. * delta.y/parent.height;
                newViewRect.x = newViewRect.x + pressed_p.x/parent.width * newViewRect.width * (1. - scalex);
                newViewRect.y = newViewRect.y + (1. - pressed_p.y/parent.height) * newViewRect.height * (1. - scaley);
                newViewRect.width *= scalex;
                newViewRect.height *= scaley;
            } else {
                // pan mode
                newViewRect.x -= delta.x * (newViewRect.width/parent.width);
                newViewRect.y += delta.y * (newViewRect.height/parent.height);
            }
            viewRect = newViewRect;
        }
        onReleased: {
        }
    }
}
