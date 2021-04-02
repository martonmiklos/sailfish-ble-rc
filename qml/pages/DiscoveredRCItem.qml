import QtQuick 2.0
import Sailfish.Silica 1.0

ListItem {
    id: detectedDevice
    property string name : ""
    property string typeName : ""
    property string image : ""
    property int index: -1

    onClicked: {
        AvailableDevicesModel.useDevice(index)
        pageStack.push(Qt.resolvedUrl("DrivePage.qml"))
    }

    Row {
        id: row
        Column {
            Text {
                id: nameText
                text: detectedDevice.name
                font.pixelSize: Theme.fontSizeLarge
            }

            Text {
                id: typeNameText
                text: detectedDevice.typeName
                font.pixelSize: Theme.fontSizeSmall
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
}
