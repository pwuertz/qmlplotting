import QtQuick 2.0

Item {
    id: selection
    property rect rect: Qt.rect(.25, .25, .5, .5)
    property color color: "gray"
    property Item image: parent
    property bool movable: true

    Connections {
        target: image
        onViewRectChanged: selection._update_position()
        onWidthChanged: selection._update_position()
        onHeightChanged: selection._update_position()
    }
    onRectChanged: _update_position()
    onImageChanged: _update_position()

    function _update_position() {
        if (!image) return;
        var srect = image.mapRectToScene(rect)
        x = srect.x; y = srect.y; width = srect.width; height = srect.height
    }

    // fade out background when selection fills the screen
    onXChanged: _update_opacity()
    onYChanged: _update_opacity()
    onWidthChanged: _update_opacity()
    onHeightChanged: _update_opacity()
    function _update_opacity() {
        var w = Math.min(x+width, image.width) - Math.max(x, 0)
        var h = Math.min(y+height, image.height) - Math.max(y, 0)
        var area = 0.
        if (w > 0 && h > 0) {
            area = w*h / (image.width*image.height)
        }
        var o = Math.min(1.-area, .3)
        rectangle.color = Qt.rgba(color.r, color.g, color.b, o)
    }

    // rectangle for visualizing the selection
    Rectangle {
        id: rectangle
        color: Qt.rgba(parent.color.r, parent.color.g, parent.color.b, .3)
        anchors.fill: parent
        anchors.margins: -1
        border.width: 2
        border.color: Qt.rgba(parent.color.r, parent.color.g, parent.color.b, .95)
        radius: 3
    }

    // mouse area for moving the selection
    MouseArea {
        id: mouse_area
        anchors.fill: parent
        anchors.margins: -1
        property point p_ref
        property bool dragging: false
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        enabled: selection.movable

        onPressed: {
            dragging = true
            p_ref = image.mapPointFromScene(Qt.point(parent.x+mouse.x, parent.y+mouse.y));
        }
        onPositionChanged: {
            if (!dragging) return
            var p_new = image.mapPointFromScene(Qt.point(parent.x+mouse.x, parent.y+mouse.y));
            p_new.x = Math.min(Math.max(0., p_new.x), 1.)
            p_new.y = Math.min(Math.max(0., p_new.y), 1.)
            var old_rect = parent.rect
            var rect = Qt.rect(
                old_rect.x + p_new.x - p_ref.x, old_rect.y + p_new.y - p_ref.y,
                old_rect.width, old_rect.height
            );
            rect.x = Math.max(Math.min(rect.x, 1.-rect.width), 0.)
            rect.y = Math.max(Math.min(rect.y, 1.-rect.height), 0.)
            parent.rect = rect;
            p_ref = p_new;
        }
        onReleased: dragging = false
        onDoubleClicked: {
            image.viewAnimation = true
            image.viewRect = selection.rect
        }
    }

    // handle for resizing the selection
    Rectangle {
        id: resize_handle
        visible: mouse_area.containsMouse || mouse_area_resize.dragging
        width: 15
        height: 15
        radius: rectangle.radius
        x: parent.width - width
        y: parent.height - height
        color: parent.color

        MouseArea {
            id: mouse_area_resize
            anchors.fill: resize_handle
            property bool dragging: false

            onPressed: {
                dragging = true
            }
            onPositionChanged: {
                if (!dragging) return
                var s_new_w = Math.max(parent.x + mouse.x, resize_handle.width)
                var s_new_h = Math.max(parent.y + mouse.y, resize_handle.height)
                var s_new_rect = Qt.rect(selection.x, selection.y, s_new_w, s_new_h)
                var new_rect = image.mapRectFromScene(s_new_rect)
                new_rect.width = Math.min(1. - new_rect.x, new_rect.width)
                new_rect.height = Math.min(1. - new_rect.y, new_rect.height)
                selection.rect = new_rect
            }
            onReleased: dragging = false
        }
    }
}
