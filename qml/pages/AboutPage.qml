import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    id: aboutPage

    allowedOrientations: window.allowedOrientations

    Flickable {
        anchors.fill: parent
        contentHeight: aboutColumn.height
        boundsBehavior: Flickable.StopAtBounds

        Column {
            id: aboutColumn

            width: parent.width
            height: childrenRect.height

            PageHeader {
                // No need to translate this one
                title: "SailRC"
            }

            LabelText {
                text: qsTr("This is a native universal application controlling BLE toy gadgets for Sailfish OS")
            }

            LabelText {
                label: qsTr("Version")
                text: Qt.application.version
            }

            LabelText {
                label: qsTr("Author")
                text: "Miklós Márton"
            }

            LabelText {
                label: qsTrId( "Special thanks to")
                text: qsTrId("<a href='https://gist.github.com/scrool'>@scrool</a> for the Brandbase car protocol")
            }
            Item {
                width: 1
                height: Theme.paddingLarge
            }
        }

        VerticalScrollDecorator { }
    }
}
