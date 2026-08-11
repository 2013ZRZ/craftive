import QtQuick
import QtQuick.Layouts
import md3.Core
import crt

Flickable {
    id: homePage
    contentWidth: width
    contentHeight: content.implicitHeight + 64
    clip: true

    ColumnLayout {
        id: content
        width: Math.min(parent.width - 48, 800)
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
            height: explore.implicitHeight
            radius: 24
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
                    itemWidth: parent.width * 0.9
                    itemHeight: itemWidth * 0.5
                    radius: 24

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

                    delegate: Image {
                        property var modelData
                        anchors.fill: parent
                        source: modelData.image
                        fillMode: Image.PreserveAspectCrop
                        horizontalAlignment: Image.AlignHCenter
                        verticalAlignment: Image.AlignVCenter
                    } // TODO rounded corner
                }

                RowLayout {
                    Layout.fillWidth: true
                    // TODO left margin
                    Text {
                        id: cardTitle
                        text: exploreCarousel.model[exploreCarousel.currentIndex].title
                        font: Typofont.titleMedium
                        Layout.alignment: Qt.AlignVCenter
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    IconButton {
                        id: exploreCarouselPrev
                        icon: "chevron_left"
                        enabled: exploreCarousel.currentIndex !== 0 // first
                        Layout.alignment: Qt.AlignVCenter
                        onClicked: exploreCarousel.currentIndex--
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
