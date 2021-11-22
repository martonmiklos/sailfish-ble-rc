import QtQuick 2.0
import Sailfish.Silica 1.0
import hu.mm.sailfish_ble_rc 1.0

Page {
    id: searchPage

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    Component {
        id: detectedDeviceDelegate
        DiscoveredRCItem {
            width: parent.width
            name: Name
            typeName: TypeName
            image: ImagePath
            index: Index
        }


    }

    SilicaListView {
        PullDownMenu {
            MenuItem {
                text: qsTr("Settings")
                onClicked: AvailableDevicesModel.detectDevices()
            }

            MenuItem {
                text: qsTr("Discover devices")
                enabled: !AvailableDevicesModel.scanInProgress
                onClicked: AvailableDevicesModel.detectDevices()
            }
        }

        id: listViewDetectedDevices
        model: AvailableDevicesModel
        delegate: detectedDeviceDelegate
        anchors.fill: parent
        leftMargin: Theme.paddingMedium
        header: PageHeader {
            title: AvailableDevicesModel.statusString
        }

        VerticalScrollDecorator {}
    }
}
