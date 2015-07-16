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
            viewRect.x = viewRect.x + wheel.x/parent.width * viewRect.width * (1. - scale);
            viewRect.y = viewRect.y + (1. - wheel.y/parent.height) * viewRect.height * (1. - scale);
            viewRect.width *= scale;
            viewRect.height *= scale;
        }
        onPressed: {
            pressed_p = Qt.point(mouse.x, mouse.y);
            old_p = Qt.point(mouse.x, mouse.y);
            zoom_mode = pressedButtons & Qt.RightButton;
        }
        onPositionChanged: {
            var delta = Qt.point(mouse.x - old_p.x, mouse.y - old_p.y);
            old_p = Qt.point(mouse.x, mouse.y);
            if (zoom_mode) {
                // zoom mode
                var scalex = 1. - 2. * delta.x/parent.width;
                var scaley = 1. + 2. * delta.y/parent.height;
                viewRect.x = viewRect.x + pressed_p.x/parent.width * viewRect.width * (1. - scalex);
                viewRect.y = viewRect.y + (1. - pressed_p.y/parent.height) * viewRect.height * (1. - scaley);
                viewRect.width *= scalex;
                viewRect.height *= scaley;
            } else {
                // pan mode
                viewRect.x -= delta.x * (viewRect.width/parent.width);
                viewRect.y += delta.y * (viewRect.height/parent.height);
            }

        }
        onReleased: {
        }
    }
}
