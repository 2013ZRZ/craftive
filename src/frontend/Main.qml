import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import md3.Core

Window {
    id: craftiveWindow
    width: 800
    height: 600
    visible: true
    title: "Craftive"
    color: Theme.color.background

    Item {
        id: root
        width: parent.width
        height: parent.height

        RowLayout {
            anchors.fill: parent
            spacing: 0

            NavigationRail {
                id: navRail
                Layout.fillHeight: true

                property bool isRail: true
                extended: !isRail

                model: [
                    {
                        icon: "home",
                        text: qsTr("Home")
                    },
                    {
                        icon: "store",
                        text: qsTr("Store")
                    },
                    {
                        icon: "play_circle",
                        text: qsTr("Play")
                    },
                    {
                        icon: "brush",
                        text: qsTr("Craftboard")
                    },
                    {
                        icon: "face",
                        text: qsTr("Myself")
                    }
                ]

                currentIndex: contentStack.currentIndex
                onItemClicked: index => contentStack.currentIndex = index

                footer: Component {
                    // Menu Button
                    Item {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 64

                        IconButton {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: isRail ? (parent.width - width) / 2 : 12

                            icon: "menu"
                            onClicked: isRail = !isRail

                            Behavior on anchors.leftMargin {
                                NumberAnimation {
                                    duration: 200
                                    easing.type: Easing.OutCubic
                                }
                            }
                        }
                    }
                }
            }

            StackLayout {
                id: contentStack
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: 0
            }
        }
    }
}
