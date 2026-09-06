pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import md3.Core

Window {
    id: craftiveWindow
    width: 800
    height: 600
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: "Craftive"
    color: Theme.color.background

    // All available pages. 0 ~ 4 are in the navigation rail and after 4 is/are not.
    enum PagesEnum {
        HomePage,
        StorePage,
        PlayPage,
        CraftboardPage,
        MyselfPage,
        SettingsPage,
        AboutPage
    }

    property int currentPage: Main.PagesEnum.HomePage
    property int prevPage: Main.PagesEnum.HomePage

    function navigatePageTo(newPage: int) {
        prevPage = currentPage;
        if (newPage < 5)  // in navigation rail
            currentPage = newPage;
        else
            currentPage = newPage;
    }

    function navigatePageBack() {
        navigatePageTo(prevPage);
    } // TODO multistage back

    Connections {
        target: exceptReceiver
        function onExceptionOccurred(which, what, how) {
            ErrDialog.show(which, what, how);
        }
    }

    Item {
        id: root
        width: parent.width
        height: parent.height

        property list<string> pagesPaths: ["pages/Home.qml", "pages/Store.qml", "pages/Play.qml", "pages/Craftboard.qml", "pages/Myself.qml", "pages/Settings.qml", "pages/About.qml"] // ordered by PagesEnum

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

                currentIndex: 0
                onItemClicked: index => {
                    currentIndex = index;
                    currentPage = index;
                }

                footer: Component {
                    ColumnLayout {
                        width: parent.width
                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 64

                            IconButton {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: navRail.isRail ? (parent.width - width) / 2 : 12

                                icon: navRail.isRail ? "chevron_right" : "chevron_left"
                                onClicked: navRail.isRail = !navRail.isRail

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

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Theme.color.background
                clip: true

                Loader {
                    id: pageLoader
                    anchors.fill: parent
                    source: root.pagesPaths[craftiveWindow.currentPage]

                    onLoaded: {
                        if (item) {
                            enterAnim.stop();
                            animOpacity.target = item;
                            animY.target = item;
                            item.opacity = 0;
                            item.y = 50;
                            enterAnim.start();
                        }
                    }
                }

                ParallelAnimation {
                    id: enterAnim
                    NumberAnimation {
                        id: animOpacity
                        property: "opacity"
                        to: 1
                        duration: 300
                        easing.type: Easing.OutCubic
                    }
                    NumberAnimation {
                        id: animY
                        property: "y"
                        to: 0
                        duration: 300
                        easing.type: Easing.OutCubic
                    }
                }
            }
        }
    }
}
