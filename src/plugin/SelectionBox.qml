import QtQuick 2.0

Item {
    id: selection
    property rect selectRect: Qt.rect(.25, .25, .5, .5)
    property color color: "gray"
    property bool movable: true

    Connections {
        target: parent
        onViewRectChanged: _update_position()
        onWidthChanged: _update_position()
        onHeightChanged: _update_position()
    }
    onSelectRectChanged: _update_position()

    function _update_position() {
        var wscale = parent.width / parent.viewRect.width
        var hscale = parent.height / parent.viewRect.height
        selection.x = (selectRect.x - parent.viewRect.x) * wscale
        selection.y = parent.height - (selectRect.y + selectRect.height - parent.viewRect.y) * hscale
        selection.width = selectRect.width * wscale
        selection.height = selectRect.height * hscale
    }

    function mapPointFromScene(x, y) {
        var wscale = parent.viewRect.width / parent.width
        var hscale = parent.viewRect.height / parent.height
        return Qt.point(
                    parent.viewRect.x + (selection.x + x)*wscale,
                    parent.viewRect.y + (parent.height - selection.y - y)*hscale
        )
    }

    function mapDistanceFromScene(w, h) {
        var wscale = parent.viewRect.width / parent.width
        var hscale = parent.viewRect.height / parent.height
        return Qt.point(w*wscale, h*hscale)
    }

    function mapRectFromScene(x, y, w, h) {
        var wscale = parent.viewRect.width / parent.width
        var hscale = parent.viewRect.height / parent.height
        return Qt.rect(
                    parent.viewRect.x + (selection.x + x)*wscale,
                    (parent.height - selection.y - y)*hscale  - parent.viewRect.y + selectRect.height,
                    w * wscale, h * hscale
        )
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
        property point p_ref: Qt.point(0, 0)
        property bool dragging: false
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        enabled: selection.movable

        onPressed: {
            dragging = true
            p_ref = mapPointFromScene(mouse.x, mouse.y)
        }
        onPositionChanged: {
            if (!dragging) return
            var p_new = mapPointFromScene(mouse.x, mouse.y)
            var delta = Qt.point(p_new.x - p_ref.x, p_new.y - p_ref.y)
            var newSelectRect =
            selectRect = Qt.rect(
                        selectRect.x + delta.x,
                        selectRect.y + delta.y,
                        selectRect.width, selectRect.height
            )
            p_ref = p_new;
        }
        onReleased: dragging = false
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
                var new_width = Math.max(resize_handle.x + mouse.x, resize_handle.width)
                var new_height = Math.max(resize_handle.y + mouse.y, resize_handle.height)
                var size = mapDistanceFromScene(new_width, new_height)
                selectRect.y = selectRect.y + selectRect.height - size.y
                selectRect.width = size.x;
                selectRect.height = size.y;
            }
            onReleased: dragging = false
        }
    }
}
