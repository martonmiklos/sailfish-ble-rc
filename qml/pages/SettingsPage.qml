import QtQuick 2.6
import Sailfish.Silica 1.0
import hu.mm.sailfish_ble_rc 1.0

Page {
    id: page

    allowedOrientations: Orientation.All


    Column {
        spacing: 10
        anchors.fill: parent
        PageHeader {
            title: qsTr("Settings")
        }
        TextSwitch {
            id: autoDiscoverBt
            checked: Settings.autoDiscoverBlDevices
            text: qsTr("Discover Bluetooth devices on startup")
            onCheckedChanged: {
                Settings.setAutoDiscoverBlDevices(autoDiscoverBt.checked)
            }
        }
        TextSwitch {
            id: leftHandedOperation
            checked: Settings.leftHandedOperation
            text: qsTr("Left handed operation")
            onCheckedChanged: {
                Settings.setLeftHandedOperation(leftHandedOperation.checked)
            }
        }
    }
}
