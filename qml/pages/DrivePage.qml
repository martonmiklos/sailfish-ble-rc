import QtQuick 2.0
import Sailfish.Silica 1.0
import "../QMLVirtualJoystick"
import hu.mm.sailfish_ble_rc 1.0

Page {
    id: page

    forwardNavigation: false
    backNavigation: false
    property bool leftHanded: false

    allowedOrientations: Orientation.Landscape | Orientation.LandscapeInverted

    VirtualJoystick {
        id: throttle
        verticalOnly: true
        height: page.height / 3
        anchors.left: parent.left
        anchors.leftMargin: Theme.paddingLarge + (parent.width / 10.0)
        anchors.verticalCenter: parent.verticalCenter
        onJoystick_moved: {
            AvailableDevicesModel.currentDevice().setThrottle(y)
        }
    }

    VirtualJoystick {
        id: steer
        horizontalOnly: true
        height: page.height / 3
        anchors.right: parent.right
        anchors.rightMargin: Theme.paddingLarge + (parent.width / 10.0)
        anchors.verticalCenter: parent.verticalCenter
        onJoystick_moved: {
            AvailableDevicesModel.currentDevice().setSteer(x)
        }
    }

    Button {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.centerIn: parent
        Image {
            anchors.fill: parent
            source: ":/res/go-previous.svg"
            fillMode: Image.Tile
        }

        onClicked: {
            AvailableDevicesModel.disconnectFromCurrentDevice()
            pageStack.push(Qt.resolvedUrl("SearchPage.qml"));
        }
    }
}
