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

    Connections {
        target: AvailableDevicesModel
        onCurrentDeviceConnectionStateChanged: {
            if (newState == AbstractRcCar.Disconnected)
                pageStack.push(Qt.resolvedUrl("SearchPage.qml"));
            else if (newState == AbstractRcCar.Connected)
                pageStack.push(Qt.resolvedUrl("DrivePage.qml"));
            else if (newState == AbstractRcCar.Connecting)
                pageStack.push(Qt.resolvedUrl("ConnectingPage.qml"));
        }
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
            mac: MacAddress
        }
    }

    SilicaListView {
        PullDownMenu {
            MenuItem {
                text: qsTr("About SailRC")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }

            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }

            MenuItem {
                text: AvailableDevicesModel.scanInProgress
                      ? qsTr("Discovery in progress...")
                      : qsTr("Discover devices")
                enabled: !AvailableDevicesModel.scanInProgress
                onClicked: AvailableDevicesModel.discoverDevices()
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
