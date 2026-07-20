import QtQuick
import QtQuick.Window
import md3.Core

Item {
    id: root
    width: parent.width
    height: parent.height

    property bool isRail: true

    RowLayout {
        anchors.fill: parent
        spacing: 0

        NavigationRail {
            id: navRail
            Layout.fillHeight: true

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

            header: Component {
                ColumnLayout {
                    width: parent.width
                    spacing: 0

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
        }

        StackLayout {
            id: contentStack
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: 0
        }
    }
}
