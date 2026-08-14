import QtQuick
import QtQuick.Layouts
import QtQuick.Effects
import md3.Core
import crt

Flickable {
    id: homePage
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
            text: qsTr("Home")
            font: Typofont.headlineLarge
            color: Theme.color.onSurfaceColor
        }

        // Explore
        Text {
            text: qsTr("Explore")
            font: Typofont.titleMedium
            color: Theme.color.primary
        }

        Rectangle {
            id: exploreCard
            Layout.fillWidth: true
            height: explore.height
            Layout.preferredHeight: explore.implicitHeight
            radius: 16
            color: Theme.color.surfaceVariant
            Layout.alignment: Qt.AlignVCenter

            ColumnLayout {
                id: explore
                width: parent.width

                Item {
                    Layout.fillWidth: true
                    height: exploreCard.radius
                }

                Carousel {
                    id: exploreCarousel
                    type: "centered"
                    itemWidth: Math.min(parent.width * 0.9, 800)
                    itemHeight: itemWidth * 0.5

                    model: [
                        {
                            image: "qrc:/images/intro.png",
                            title: qsTr("Welcome to Craftive!") + " (0)"
                        },
                        {
                            image: "qrc:/images/intro.png",
                            title: qsTr("Welcome to Craftive!") + " (1)"
                        },
                        {
                            image: "qrc:/images/intro.png",
                            title: qsTr("Welcome to Craftive!") + " (2)"
                        }
                    ]

                    delegate: Item {
                        property var modelData
                        anchors.fill: parent

                        Image {
                            id: sourceImage
                            anchors.fill: parent
                            source: modelData.image
                            fillMode: Image.PreserveAspectCrop
                            horizontalAlignment: Image.AlignHCenter
                            verticalAlignment: Image.AlignVCenter

                            layer.enabled: true
                            layer.effect: MultiEffect {
                                maskEnabled: true
                                maskSource: carouselMask
                                autoPaddingEnabled: false
                                antialiasing: true
                                maskThresholdMin: 0.5
                                maskSpreadAtMin: 1.0
                            }
                        }

                        Rectangle {
                            id: carouselMask
                            anchors.fill: parent
                            radius: 16
                            color: "black"
                            visible: false
                            layer.enabled: true
                        }

                        LoadingIndicator {
                            anchors.centerIn: parent
                            size: 48
                            running: sourceImage.status === Image.Loading
                            visible: sourceImage.status === Image.Loading
                            withContainer: true
                            z: 99
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    IconButton {
                        id: exploreCarouselPrev
                        icon: "chevron_left"
                        enabled: exploreCarousel.currentIndex !== 0 // first
                        Layout.alignment: Qt.AlignVCenter
                        onClicked: exploreCarousel.currentIndex--
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    Text {
                        id: cardTitle
                        text: {
                            var result = exploreCarousel.model[exploreCarousel.currentIndex].title;
                            var maxsize = Math.floor((exploreCard.width - 80) * 0.9 / cardTitle.font.pixelSize);
                            return (result.length > maxsize) ? (result.substring(0, maxsize) + "...") : result;
                        }
                        font: Typofont.titleMedium
                        color: Theme.color.onSurfaceVariantColor
                        Layout.alignment: Qt.AlignCenter
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    IconButton {
                        id: exploreCarouselNext
                        icon: "chevron_right"
                        enabled: exploreCarousel.currentIndex !== exploreCarousel.count - 1 // last
                        Layout.alignment: Qt.AlignVCenter
                        onClicked: exploreCarousel.currentIndex++
                    }
                }
            }
        }
    }

    ScrollBar {
        anchors.right: parent.right
        target: homePage
        orientation: Qt.Vertical
    }
}
