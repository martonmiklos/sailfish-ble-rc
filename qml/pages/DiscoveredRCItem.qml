import QtQuick 2.0
import Sailfish.Silica 1.0
import hu.mm.sailfish_ble_rc 1.0

ListItem {
    id: detectedDevice
    property string name : ""
    property string typeName : ""
    property string image : ""
    property int index: -1

    onClicked: {
        AvailableDevicesModel.connectToDevice(index)
        pageStack.push(Qt.resolvedUrl("ConnectingPage.qml"))
    }

    Row {
        id: row
        Column {
            Text {
                id: nameText
                text: detectedDevice.name
                font.pixelSize: Theme.fontSizeLarge
                font.family: Theme.fontFamily
                color: Theme.primaryColor
            }

            Text {
                id: typeNameText
                text: detectedDevice.typeName
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
            }
            width: (detectedDevice.width - row.height - Theme.paddingLarge)
        }
        Image {
            id: name
            source: detectedDevice.image
            height: row.height
            width: row.height
            fillMode: Image.PreserveAspectFit
        }
    }
    menu: ContextMenu {
        MenuItem {
            text: qsTr("Autoconnect")
        }
    }
}
