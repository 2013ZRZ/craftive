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

        RowLayout {
            Item {
                Layout.fillWidth: true
            }
            IconButton {
                icon: "settings"
                onClicked: navigatePageTo(Main.PagesEnum.SettingsPage)
            }
        }
    }
}
