pragma Singleton
import QtQuick
import md3.Core

Dialog {
    id: globalErrDialog
    title: qsTr("Oops! Error %1 \"%2\" occurred.")
    text: qsTr("Detail: %1")
    icon: "error"
    acceptText: qsTr("Retry")
    rejectText: qsTr("Cancel")

    function show(which: string, what: string, how: string) {
        title = title.arg(which).arg(what);
        text = text.arg(how);
        this.open();
    }

    onClosed: {
        title = qsTr("Oops! Error %1 \"%2\" occurred.");
        text = qsTr("Detail: %1");
    }
}
