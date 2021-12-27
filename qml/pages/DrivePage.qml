import QtQuick 2.6
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
        height: page.height / 2
        anchors.left: parent.left
        anchors.leftMargin: Theme.paddingSmall
        anchors.verticalCenter: parent.verticalCenter
        onJoystick_moved: {
            if (AvailableDevicesModel.currentDevice() !== null)
            AvailableDevicesModel.currentDevice().setThrottle(y)
        }
    }


    Column {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: Theme.paddingMedium
        topPadding: Theme.paddingLarge

        Text {
            id: batteryLevel
            font.pixelSize: Theme.fontSizeMedium
            color: Theme.primaryColor
            anchors.horizontalCenter: parent.horizontalCenter
            visible: AvailableDevicesModel.currentDevice().isFeatureSupported(AbstractRcCar.BatteryVoltageQuery)
            Connections {
                target: AvailableDevicesModel.currentDevice()
                onBatteryPercentageUpdated: {
                    batteryLevel.text = (AvailableDevicesModel.currentDevice().batteryPercentage().toString() + "%")
                }
            }
        }

        Button {
            id: backButton
            anchors.horizontalCenter: parent.horizontalCenter
            width: height
            Image {
                anchors.fill: parent
                source: "qrc:/res/go-previous.svg"
                fillMode: Image.Stretch
            }

            onClicked: {
                AvailableDevicesModel.disconnectFromCurrentDevice()
                pageStack.push(Qt.resolvedUrl("SearchPage.qml"));
            }
        }

        Button {
            id: turboButton
            width: backButton.width
            anchors.horizontalCenter: parent.horizontalCenter
            visible: AvailableDevicesModel.currentDevice().isFeatureSupported(AbstractRcCar.TurboMode)
            onClicked: {
                AvailableDevicesModel.currentDevice().setFeature(
                            AbstractRcCar.TurboMode,
                            !AvailableDevicesModel.currentDevice().featureValue(AbstractRcCar.TurboMode))
                turboImage.source = AvailableDevicesModel.currentDevice().featureValue(AbstractRcCar.TurboMode)
                        ? "qrc:/res/turbo_active.png"
                        : "qrc:/res/turbo.png"
            }

            Image {
                id: turboImage
                anchors.fill: parent
                source: AvailableDevicesModel.currentDevice().featureValue(AbstractRcCar.TurboMode)
                        ? "qrc:/res/turbo_active.png"
                        : "qrc:/res/turbo.png"
                fillMode: Image.Stretch
            }
        }
    }

    VirtualJoystick {
        id: steer
        horizontalOnly: true
        height: page.height / 2
        anchors.right: parent.right
        anchors.rightMargin: Theme.paddingSmall
        anchors.verticalCenter: parent.verticalCenter
        onJoystick_moved: {
            if (AvailableDevicesModel.currentDevice() !== null)
                AvailableDevicesModel.currentDevice().setSteer(x)
        }
    }
}
