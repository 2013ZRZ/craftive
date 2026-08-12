import QtQuick
import QtQuick.Layouts
import QtQuick.Effects
import md3.Core
import crt

Flickable {
    id: settingsPage
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

        Text {
            text: qsTr("Settings")
            font: Typofont.headlineLarge
            color: Theme.color.onSurfaceColor
        }

        // Appearance Section
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 16

            Text {
                text: qsTr("Appearance")
                font: Typofont.titleMedium
                color: Theme.color.primary
            }

            // Dark Mode
            Rectangle {
                Layout.fillWidth: true
                height: 72
                color: Theme.color.surfaceContainer
                radius: 12

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    IconCircle {
                        icon: StyleManager.isDarkTheme ? "dark_mode" : "light_mode"
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        Text {
                            text: qsTr("Theme")
                            font: Typofont.titleSmall
                            color: Theme.color.onSurfaceColor
                        }
                        Text {
                            text: StyleManager.isDarkTheme ? qsTr("Using dark theme now") : qsTr("Using light theme now")
                            font: Typofont.bodySmall
                            color: Theme.color.onSurfaceVariantColor
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Switch {
                        icon: "dark_mode"
                        checked: StyleManager.isDarkTheme
                        onClicked: StyleManager.isDarkTheme = checked
                    }
                }
            }

            // Theme Color
            Rectangle {
                Layout.fillWidth: true
                implicitHeight: themeColumn.implicitHeight + 32
                color: Theme.color.surfaceContainer
                radius: 12

                ColumnLayout {
                    id: themeColumn
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    RowLayout {
                        spacing: 16
                        IconCircle {
                            icon: "palette"
                        }
                        ColumnLayout {
                            spacing: 2
                            Text {
                                text: qsTr("Theme Color")
                                font: Typofont.titleSmall
                                color: Theme.color.onSurfaceColor
                            }
                            Text {
                                text: qsTr("Select a seed color to generate the Material You color scheme")
                                font: Typofont.bodySmall
                                color: Theme.color.onSurfaceVariantColor
                            }
                        }
                    }

                    GridLayout {
                        Layout.fillWidth: true
                        columns: Math.floor((parent.width - 32) / 52)
                        rowSpacing: 12
                        columnSpacing: 12

                        Repeater {
                            model: ["#6750a4", "#9C27B0", "#E91E63", "#F44336", "#b3261e", "#795548", "#3F51B5", "#2196F3", "#00BCD4", "#009688", "#4CAF50", "#8BC34A", "#CDDC39", "#FFEB3B", "#FFC107", "#FF9800"]

                            delegate: Item {
                                width: 40
                                height: 40

                                Rectangle {
                                    anchors.centerIn: parent
                                    width: 40
                                    height: 40
                                    radius: 20
                                    color: modelData

                                    // Selection Indicator
                                    Rectangle {
                                        anchors.fill: parent
                                        anchors.margins: -4
                                        radius: 24
                                        color: "transparent"
                                        border.width: 3
                                        border.color: Theme.color.primary
                                        visible: Qt.colorEqual(StyleManager.seedColor, modelData)

                                        // Animation
                                        Behavior on border.width {
                                            NumberAnimation {
                                                duration: 150
                                            }
                                        }
                                    }

                                    // Checkmark
                                    Text {
                                        anchors.centerIn: parent
                                        text: "check"
                                        font.family: Theme.iconFont.name
                                        font.pixelSize: 24
                                        color: (0.299 * StyleManager.seedColor.r + 0.587 * StyleManager.seedColor.g + 0.114 * StyleManager.seedColor.b) > 0.5 ? "black" : "white"
                                        visible: Qt.colorEqual(StyleManager.seedColor, modelData)
                                        opacity: visible ? 1 : 0
                                        scale: visible ? 1 : 0.5
                                        Behavior on scale {
                                            NumberAnimation {
                                                duration: 200
                                                easing.type: Easing.OutBack
                                            }
                                        }
                                        Behavior on opacity {
                                            NumberAnimation {
                                                duration: 150
                                            }
                                        }
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: StyleManager.seedColor = modelData
                                    }
                                }
                            }
                        }
                    }

                    // Custom Color Picker
                    ColorPicker {
                        Layout.fillWidth: true
                        Layout.topMargin: 8
                        title: qsTr("Or select more precisely below:")
                        hasAlpha: false
                        asSeedColor: true
                        selectedColor: StyleManager.seedColor
                        hueText: qsTr("Hue")
                        chromaText: qsTr("Chroma")
                        toneText: qsTr("Tone")
                    }
                }
            }
        }

        // About Section
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 16

            Text {
                text: qsTr("About")
                font: Typofont.titleMedium
                color: Theme.color.primary
            }

            Rectangle {
                Layout.fillWidth: true
                height: 72
                color: Theme.color.surfaceContainer
                radius: 12

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    IconCircle {
                        icon: "info"
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        Text {
                            text: qsTr("Version")
                            font: Typofont.titleSmall
                            color: Theme.color.onSurfaceColor
                        }
                        Text {
                            text: "Dev"
                            font: Typofont.bodySmall
                            color: Theme.color.onSurfaceVariantColor
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }

    ScrollBar {
        anchors.right: parent.right
        target: settingsPage
        orientation: Qt.Vertical
    }
}
