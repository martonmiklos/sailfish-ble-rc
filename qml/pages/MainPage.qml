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
        anchors.verticalCenter: parent.verticalCenter
    }

    VirtualJoystick {
        id: steer
        horizontalOnly: true
        height: page.height / 3
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
    }
}
