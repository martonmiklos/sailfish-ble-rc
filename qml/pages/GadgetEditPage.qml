import QtQuick 2.6
import Sailfish.Silica 1.0
import hu.mm.sailfish_ble_rc 1.0

Page {
    id: gadgetEditorPage

    allowedOrientations: Orientation.All
    property string gadgetMac: ""

    Column {
        spacing: 10
        anchors.fill: parent
        PageHeader {
            title: qsTr("Edit gadget details")
        }
        TextField {
            id: gadgetAlias
            focus: true
            text: Settings.alias(gadgetMac)
            placeholderText: qsTr("This text will appear instead of the MAC in search")
        }
        Button {
            text: qsTr("Save")
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                Settings.setAlias(gadgetMac, gadgetAlias.text)
                pageStack.push(Qt.resolvedUrl("SearchPage.qml"))
            }
        }
    }
}
