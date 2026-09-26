import QtQuick
import QtQuick.Layouts
import md3.Core
import crt

Flickable {
    id: myselfPage
    contentWidth: width
    contentHeight: content.implicitHeight + 64
    clip: true

    ColumnLayout {
        id: content
        width: parent.width - 48
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 32
        spacing: 24

        // Manage the loaded kits
        Rectangle {
            Layout.fillWidth: true
            height: 72
            color: Theme.color.surfaceContainer
            radius: 16

            RowLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 16

                IconCircle {
                    icon: "app_registration"
                }
                Text {
                    text: qsTr("Manage the loaded kits")
                    font: Typofont.titleSmall
                    color: Theme.color.onSurfaceColor
                }
                Item {
                    Layout.fillWidth: true
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: manageLoadedKitsDialog.open()
            }
        }

        Dialog {
            id: manageLoadedKitsDialog
            icon: "app_registration"
            title: qsTr("Manage the loaded kits")
            showAcceptButton: false
            rejectText: qsTr("Close")

            ListView {
                id: manageLoadedKitsList
                width: parent.width
                // Note: contentHeight is -1 while the model is empty
                height: count === 0 ? 72 : Math.min(contentHeight, 320)
                clip: true
                boundsBehavior: Flickable.StopAtBounds
                model: QmlStatus.loadedKits

                Text {
                    anchors.centerIn: manageLoadedKitsList
                    width: manageLoadedKitsList.width - 32
                    visible: manageLoadedKitsList.count === 0
                    text: qsTr("No kit is loaded yet.")
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.Wrap
                    font: Typofont.bodyMedium
                    color: Theme.color.onSurfaceVariantColor
                }

                ScrollBar {
                    target: manageLoadedKitsList
                    orientation: Qt.Vertical
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                }

                delegate: Card {
                    id: kitCard

                    required property string name
                    required property string id
                    required property int index

                    width: manageLoadedKitsList.width
                    height: 72
                    radius: 36

                    RowLayout {
                        anchors.fill: parent
                        spacing: 16

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 2
                            Text {
                                Layout.fillWidth: true
                                text: kitCard.name
                                elide: Text.ElideRight
                                font: Typofont.titleSmall
                                color: Theme.color.onSurfaceColor
                            }
                            Text {
                                Layout.fillWidth: true
                                text: kitCard.id
                                elide: Text.ElideRight
                                font: Typofont.bodySmall
                                color: Theme.color.onSurfaceVariantColor
                            }
                        }

                        Button {
                            Layout.alignment: Qt.AlignHCenter
                            type: "filled"
                            icon: "delete"
                            text: qsTr("Remove")
                            onClicked: QmlStatus.removeKit(kitCard.id)
                        }
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2

            // Settings
            Rectangle {
                Layout.fillWidth: true
                height: 72
                color: Theme.color.surfaceContainer
                topLeftRadius: 16
                topRightRadius: 16
                bottomLeftRadius: 2
                bottomRightRadius: 2

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    IconCircle {
                        icon: "settings"
                    }
                    Text {
                        text: qsTr("Settings")
                        font: Typofont.titleSmall
                        color: Theme.color.onSurfaceColor
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: navigatePageTo(Main.PagesEnum.SettingsPage)
                }
            }

            // About
            Rectangle {
                Layout.fillWidth: true
                height: 72
                color: Theme.color.surfaceContainer
                topLeftRadius: 2
                topRightRadius: 2
                bottomLeftRadius: 16
                bottomRightRadius: 16

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    IconCircle {
                        icon: "info"
                    }
                    Text {
                        text: qsTr("About")
                        font: Typofont.titleSmall
                        color: Theme.color.onSurfaceColor
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: navigatePageTo(Main.PagesEnum.AboutPage)
                }
            }
        }
    }

    ScrollBar {
        anchors.right: parent.right
        target: myselfPage
        orientation: Qt.Vertical
    }
}
