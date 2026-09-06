import QtQuick
import QtQuick.Layouts
import QtQuick.Effects
import md3.Core

Flickable {
    id: aboutPage
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

        RowLayout {
            IconButton {
                icon: "west"
                onClicked: navigatePageBack()
            }
            Text {
                text: qsTr("About")
                font: Typofont.headlineLarge
                color: Theme.color.onSurfaceColor
            }
        }

        // Logo
        Item {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 120
            Layout.preferredHeight: 120

            Image {
                anchors.fill: parent
                source: "qrc:/images/logo.svg"
                fillMode: Image.PreserveAspectCrop
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignVCenter

                layer.enabled: true
                layer.effect: MultiEffect {
                    maskEnabled: true
                    maskSource: logoMask
                    autoPaddingEnabled: false
                    antialiasing: true
                    maskThresholdMin: 0.5
                    maskSpreadAtMin: 1.0
                }
            }

            Rectangle {
                id: logoMask
                anchors.fill: parent
                radius: 60
                color: "black"
                visible: false
                layer.enabled: true
            }
        }

        // Title
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Craftive"
            font: Typofont.displayMedium
            color: Theme.color.onSurfaceColor
        }

        // Version
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Developing..."
            font: Typofont.titleMedium
            color: Theme.color.onSurfaceColor
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2

            // Build Time
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
                        icon: "calendar_today"
                    }
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        Text {
                            text: qsTr("Build Time")
                            font: Typofont.titleSmall
                            color: Theme.color.onSurfaceColor
                        }
                        Text {
                            text: buildInfoBuildTime
                            font: Typofont.bodySmall
                            color: Theme.color.onSurfaceVariantColor
                        }
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                }
            }

            // Commit Hash
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
                        icon: "commit"
                    }
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        Text {
                            text: qsTr("Commit Hash")
                            font: Typofont.titleSmall
                            color: Theme.color.onSurfaceColor
                        }
                        Text {
                            text: buildInfoCommitHash
                            font: Typofont.bodySmall
                            color: Theme.color.onSurfaceVariantColor
                        }
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                }
            }
        }
    }

    ScrollBar {
        anchors.right: parent.right
        target: aboutPage
        orientation: Qt.Vertical
    }
}
