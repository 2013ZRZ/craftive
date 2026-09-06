import QtQuick
import QtQuick.Layouts
import md3.Core

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
