import QtQuick 2.0
import Sailfish.Silica 1.0
import "../QMLVirtualJoystick"

Page {
    id: page

    property bool leftHanded: false

    allowedOrientations: Orientation.Landscape | Orientation.LandscapeInverted

    VirtualJoystick {
        id: throttle
        verticalOnly: true
        height: page.height / 3
        anchors.left: parent.left
        anchors.leftMargin: Theme.paddingLarge
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
        anchors.rightMargin: Theme.paddingLarge
        anchors.verticalCenter: parent.verticalCenter
        onJoystick_moved: {
            AvailableDevicesModel.currentDevice().setSteer(x)
        }
    }
}
