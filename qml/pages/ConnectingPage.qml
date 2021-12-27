import QtQuick 2.0
import Sailfish.Silica 1.0
import hu.mm.sailfish_ble_rc 1.0

Page {
    id: connectingPage

    Text {
        id: connecingText
        text: qsTr("Connecting...")
        font.pixelSize: Theme.fontSizeLarge
        color: Theme.primaryColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: Theme.paddingLarge
    }

    Text {
        id: connectionStateText
        font.pixelSize: Theme.fontSizeSmall
        color: Theme.primaryColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: connecingText.bottom
        anchors.topMargin: Theme.paddingSmall
    }

    BusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: true
    }

    Button {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Theme.paddingLarge
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Cancel")
        onClicked: {
            AvailableDevicesModel.disconnectFromCurrentDevice();
            pageStack.push(Qt.resolvedUrl("SearchPage.qml"));
        }
    }

    Connections {
        target: AvailableDevicesModel
        onCurrentDeviceConnectionStateChanged: {
            if (newState == AbstractRcCar.Disconnected)
                pageStack.push(Qt.resolvedUrl("SearchPage.qml"));
            else if (newState == AbstractRcCar.Connected)
                pageStack.push(Qt.resolvedUrl("DrivePage.qml"));
        }
        onCurrentDeviceConnectionStateStringChanged: {
            console.log(stateString)
            connectionStateText.text = stateString
        }
    }
}
