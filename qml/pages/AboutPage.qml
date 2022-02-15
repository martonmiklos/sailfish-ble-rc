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
                label: qsTr("About the SailRC")
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
                label: qsTr( "Special thanks to")
                text: qsTr('<ul>' +
                                '<li><a href="https://gist.github.com/scrool">@scrool</a> for the <a href="https://gist.github.com/scrool/e79d6a4cb50c26499746f4fe473b3768">Brandbase car protocol</a></li>' +
                                '<li><a href="https://gist.github.com/aaronsnoswell">@aaronsnoswell</a> for the <a href="https://github.com/aaronsnoswell/QMLVirtualJoystick">QMLVirtualJoystick</a> component</li>' +
                             '</ul>')
            }
            Item {
                width: 1
                height: Theme.paddingLarge
            }
        }

        VerticalScrollDecorator { }
    }
}
