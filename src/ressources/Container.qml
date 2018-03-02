import QtQuick 2.7

Item {
    id: root
    property rect viewRect
    property rect itemRect: Qt.rect(0, 0, 1, 1)

    onWidthChanged: updateChildItems()
    onHeightChanged: updateChildItems()
    onViewRectChanged: updateChildItems()
    onItemRectChanged: updateChildItems()
    Component.onCompleted: updateChildItems()
    onChildrenChanged: updateChildItems()

    function updateChildItems() {
        // Calculate child item coordinates
        var xToScene = width / viewRect.width;
        var yToScene = height / viewRect.height;
        var childHeight = itemRect.height * yToScene;
        var childWidth = itemRect.width * xToScene;
        var childX = (itemRect.x - viewRect.x) * xToScene;
        var childY = height - childHeight - (itemRect.y - viewRect.y) * yToScene;
        // Apply to children
        for (var i=0; i < children.length; ++i) {
            var child = children[i];
            child.x = childX;
            child.y = childY;
            child.width = childWidth;
            child.height = childHeight;
        }
    }
}
