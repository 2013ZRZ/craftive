pragma Singleton
import QtQuick
import md3.Core
import crt

Dialog {
    id: globalErrDialog
    title: qsTr("Oops! Error %1 \"%2\" occurred.")
    icon: "error"
    acceptText: qsTr("Retry")
    rejectText: qsTr("Cancel")

    signal errorOccurred(e: CrtExcept)
    onErrorOccurred: e => {
        this.title = this.title.arg(e.whichStr).arg(e.what);
        this.text = qsTr("Detail: %1").arg(e.how);
        this.open();
    }
}
