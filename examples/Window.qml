import QtQuick 2.0
import QtQml 2.2

Rectangle {
    id: window_item
    width: contents_item.width + 2
    height: contents_item.height + 2 + top_item.height
    radius: 4
    color: "#eee"
    property string title: "Untitled"
    property bool flat: false
    default property Item contents

    border.width: 1
    border.color: Qt.darker(window_item.color, 1.3)

    Rectangle {
        id: top_item
        width: parent.width
        property int spacing: 4
        height: top_item_label.height + 2*spacing + radius
        radius: 4
        property var default_gradient: Gradient {
            GradientStop { position: 0.0; color: window_item.color }
            GradientStop { position: 0.1; color: Qt.lighter(window_item.color, 1.1) }
            GradientStop { position: 1.0; color: Qt.darker(window_item.color, 1.1) }
        }
        gradient: (window_item.flat) ? null : default_gradient;
        color: Qt.lighter(window_item.color, 1.1)
        border.width: 1
        border.color: Qt.darker(window_item.color, 1.3)

        Text {
            x: radius + top_item.spacing
            y: radius/2 + top_item.spacing
            id: top_item_label
            text: title
            color: Qt.darker(window_item.color, 4.0)
        }
        MouseArea {
            anchors.fill: parent
            drag.target: window_item
            drag.axis: Drag.XandYAxis
        }
    }

    Item {
        id: contents_item
        width: contents.width
        height: contents.height
        y: top_item.height
        x: 1
    }

    onContentsChanged: {contents.parent = contents_item;}
}
