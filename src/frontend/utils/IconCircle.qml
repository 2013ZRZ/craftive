import QtQuick
import md3.Core

Rectangle {
    required property string icon
    property real diameter: 40

    width: diameter
    height: diameter
    radius: (diameter / 2)
    color: Theme.color.surfaceContainerHigh
    Text {
        anchors.centerIn: parent
        text: parent.icon
        font.family: Theme.iconFont.name
        font.pixelSize: (parent.diameter * 0.6)
        color: Theme.color.onSurfaceVariantColor
    }
}
