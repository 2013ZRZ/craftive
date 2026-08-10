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
        Carousel {
            id: exploreCarousel
            type: "hero"
            itemWidth: 320
            itemHeight: 180

            model: [
                {
                    image: ":/images/intro.png",
                    title: qsTr("Welcome to Craftive!") + "(0)"
                },
                {
                    image: ":/images/intro.png",
                    title: qsTr("Welcome to Craftive!") + "(1)"
                },
                {
                    image: ":/images/intro.png",
                    title: qsTr("Welcome to Craftive!") + "(2)"
                }
            ]

            delegate: Item {
                width: parent.width
                height: parent.height
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                Image {
                    source: model.image
                    anchors.fill: parent
                    fillMode: PreserveAspectCrop
                }
            }
        }

        Row {
            anchors.width: parent.width
            Text {
                id: cardTitle
                text: exploreCarousel.model.title
                anchors.leftMargin: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
            Button {
                id: exploreCarouselPrev
                type: "filledTonal"
                icon: "chevron_left"
                text: qsTr("Previous")
                anchors.rightMargin: exploreCarouselNext.left
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    if (exploreCarousel.index == 0) // first
                        exploreCarousel.index = exploreCarousel.count - 1;
                    else
                        exploreCarousel.index--;
                }
            }
            Button {
                id: exploreCarouselNext
                type: "filledTonal"
                icon: "chevron_right"
                text: qsTr("Next")
                anchors.rightMargin: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    if (exploreCarousel.index == exploreCarousel.count - 1) // last
                        exploreCarousel.index = 0;
                    else
                        exploreCarousel.index++;
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
