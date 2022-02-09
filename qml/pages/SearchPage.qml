import QtQuick 2.0
import Sailfish.Silica 1.0
import hu.mm.sailfish_ble_rc 1.0

Page {
    id: searchPage

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    Component.onCompleted: {
        AvailableDevicesModel.disconnectFromCurrentDevice()
    }

    Component {
        id: detectedDeviceDelegate
        DiscoveredRcItem {
            width: parent.width
            name: Name
            typeName: TypeName
            image: ImagePath
            index: Index
            alias: Alias
        }
    }

    SilicaListView {
        PullDownMenu {
            MenuItem {
                text: qsTr("About SailRC")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"));
            }

            MenuItem {
                text: qsTr("Settings")
            }

            MenuItem {
                text: AvailableDevicesModel.scanInProgress
                      ? qsTr("Discovery in progress...")
                      : qsTr("Discover devices")
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
