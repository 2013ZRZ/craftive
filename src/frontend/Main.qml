import QtQuick
import QtQuick.Window
import md3.Core

Window {
    id: craftiveWindow
    width: 800
    height: 600
    visible: true
    title: "Craftive"
    color: Theme.color.background

    Item {
        id: prodRoot
        Loader {
            anchors.fill: parent
            source: "approot.qml"
        }
    }
}
