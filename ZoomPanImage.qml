import QtQuick 2.0
import QtQuick.Controls 1.1
import qmlplotting 1.0

ColormappedImage {
    id: image
    property alias viewAnimation: viewRectBehavior.enabled

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

    function mapPointFromScene(spoint) {
        var wscale = viewRect.width / image.width
        var hscale = viewRect.height / image.height
        var vx = viewRect.x + spoint.x*wscale
        var vy = viewRect.y + spoint.y*hscale
        return Qt.point(vx, vy)
    }

    function mapRectFromScene(srect) {
        var wscale = viewRect.width / image.width
        var hscale = viewRect.height / image.height
        var vx = viewRect.x + srect.x*wscale
        var vy = viewRect.y + srect.y*hscale
        return Qt.rect(vx, vy, srect.width*wscale, srect.height*hscale)
    }

    function mapPointToScene(vpoint) {
        var wscale = image.width / viewRect.width
        var hscale = image.height / viewRect.height
        var x = (vpoint.x-viewRect.x) * wscale
        var y = (vpoint.y-viewRect.y) * hscale
        return Qt.point(x, y)
    }

    function mapRectToScene(vrect) {
        var wscale = image.width / viewRect.width
        var hscale = image.height / viewRect.height
        var x = (vrect.x-viewRect.x) * wscale
        var y = (vrect.y-viewRect.y) * hscale
        return Qt.rect(x, y, vrect.width*wscale, vrect.height*hscale)
    }

    // mouse area for panning the view
    MouseArea {
        property real old_x: 0
        property real old_y: 0
        acceptedButtons: Qt.MiddleButton
        anchors.fill: parent
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
