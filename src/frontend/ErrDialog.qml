pragma Singleton
import QtQuick
import md3.Core
import crt

Dialog {
    id: globalErrDialog
    title: qsTr("Oops! Error %1 \"%2\" occurred.")
    text: qsTr("Detail: %1")
    icon: "error"
    acceptText: qsTr("Retry")
    rejectText: qsTr("Cancel")

    signal errorOccurred(e: CrtExcept)
    onErrorOccurred: e => {
        title = title.arg(e.whichStr).arg(e.what);
        text = text.arg(e.how);
        this.open();
    }

    onClosed: {
        title = qsTr("Oops! Error %1 \"%2\" occurred.");
        text = qsTr("Detail: %1");
    }
}
