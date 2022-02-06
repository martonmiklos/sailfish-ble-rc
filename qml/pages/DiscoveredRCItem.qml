import QtQuick 2.0
import Sailfish.Silica 1.0
import hu.mm.sailfish_ble_rc 1.0

ListItem {
    id: detectedDevice
    property string name : ""
    property string typeName : ""
    property string image : ""
    property string alias: ""
    property int index: -1
    height: row.height + Theme.paddingSmall

    onClicked: {
        AvailableDevicesModel.connectToDevice(index)
        pageStack.push(Qt.resolvedUrl("ConnectingPage.qml"))
    }

    Row {
        id: row
        spacing: Theme.paddingSmall
        anchors.leftMargin: Theme.paddingMedium
        anchors.rightMargin: Theme.paddingMedium
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

            Text {
                id: aliasText
                text: detectedDevice.alias
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
            }

            width: (detectedDevice.width - row.height * 1.2 - Theme.paddingLarge * 2)
        }
        Image {
            id: name
            source: detectedDevice.image
            height: row.height
            width: row.height * 1.2
            fillMode: Image.PreserveAspectFit
        }
    }
}
