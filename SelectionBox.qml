import QtQuick 2.0

Item {
    id: selection
    property rect rect: Qt.rect(.25, .25, .5, .5)
    property color color: "gray"
    property Item image: parent

    Connections {
        target: image
        onViewRectChanged: selection._update_position()
        onWidthChanged: selection._update_position()
        onHeightChanged: selection._update_position()
    }
    onRectChanged: _update_position()

    function _update_position() {
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
    }

    // mouse area for moving the selection
    MouseArea {
        id: mouse_area
        anchors.fill: parent
        property point p_ref
        property bool moving: false
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton

        onPressed: {
            moving = true
            p_ref = image.mapPointFromScene(Qt.point(parent.x+mouse.x, parent.y+mouse.y));
        }
        onPositionChanged: {
            if (!moving) return
            var p_new = image.mapPointFromScene(Qt.point(parent.x+mouse.x, parent.y+mouse.y));
            var old_rect = parent.rect
            var rect = Qt.rect(
                old_rect.x + p_new.x - p_ref.x, old_rect.y + p_new.y - p_ref.y,
                old_rect.width, old_rect.height
            );
            parent.rect = rect;
            p_ref = p_new;
        }
        onReleased: moving = false
        onDoubleClicked: {
            image.viewAnimation = true
            image.viewRect = selection.rect
        }
    }

    // handle for resizing the selection
    Rectangle {
        id: resize_handle
        visible: mouse_area.containsMouse
        width: 15
        height: 15
        radius: 5
        x: parent.width - width + radius
        y: parent.height - height + radius
        color: parent.color

        MouseArea {
            id: mouse_area_resize
            anchors.fill: resize_handle
            onPositionChanged: {
                var s_new_w = Math.max(parent.x + mouse.x, resize_handle.width)
                var s_new_h = Math.max(parent.y + mouse.y, resize_handle.height)
                var s_new_rect = Qt.rect(selection.x, selection.y, s_new_w, s_new_h)
                selection.rect = image.mapRectFromScene(s_new_rect)
            }
        }
    }
}
