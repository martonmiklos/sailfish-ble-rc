import QtQuick 2.0
import Sailfish.Silica 1.0
import hu.mm.sailfish_ble_rc 1.0

import "../components"

Page {
    id: gadgetInfoPage

    property string gadgetMac: ""

    allowedOrientations: window.allowedOrientations

    Flickable {
        anchors.fill: parent
        contentHeight: infoColumn.height
        boundsBehavior: Flickable.StopAtBounds

        Column {
            id: infoColumn

            width: parent.width
            height: childrenRect.height

            PageHeader {
                title: Settings.alias(gadgetMac).length === 0
                       ? gadgetMac
                       : Settings.alias(gadgetMac) + "(" + gadgetMac + ")"
            }

            LabelText {
                label: qsTr("Connection time")
                text: Settings.connectTimeString(gadgetMac)
            }

            LabelText {
                label: qsTr("Running time")
                text: Settings.runTimeString(gadgetMac)
            }

            Item {
                width: 1
                height: Theme.paddingLarge
            }
        }

        VerticalScrollDecorator { }
    }
}
