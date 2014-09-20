import QtQuick 2.0
import QtQuick.Controls 1.1
import qmlplotting 1.0

ColormappedImage {
    id: image
    property alias viewAnimation: viewRectBehavior.enabled
    property bool movable: true
    viewRect: Qt.rect(0., 0., 1., 1.)

    Behavior on viewRect { id: viewRectBehavior; PropertyAnimation {}}

    function zoom(factor) {
        var inv = 1. / factor;
        var w = viewRect.width * inv;
        var h = viewRect.height * inv;
        var dw = viewRect.width - w;
        var dh = viewRect.height - h;
        viewRect = Qt.rect(viewRect.x + dw*.5, viewRect.y + dh*.5, w, h);
    }

    // mouse area for panning the view
    MouseArea {
        property real old_x: 0
        property real old_y: 0
        acceptedButtons: Qt.MiddleButton
        anchors.fill: parent
        enabled: parent.movable

        onWheel: {
            var f = 1. + wheel.angleDelta.y * (0.25 / 120.);
            viewAnimation = false;
            zoom(f);
        }
        onPressed: {old_x = mouse.x; old_y = mouse.y;}
        onPositionChanged: {
            var dx = mouse.x - old_x;
            var dy = mouse.y - old_y;
            old_x = mouse.x; old_y = mouse.y;
            viewAnimation = false;
            viewRect.x -= dx * (viewRect.width/image.width);
            viewRect.y -= dy * (viewRect.height/image.height);
        }
    }
}
