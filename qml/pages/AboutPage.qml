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
                //: About page text
                //% "This is a native universal application controlling BLE toy gadgets for Sailfish OS"
                text: qsTrId("about-description")
            }

            LabelText {
                //: About page label
                //% "Version"
                label: qsTrId("about-version-label")
                text: Qt.application.version
            }

            LabelText {
                //: About page label
                //% "Author"
                label: qsTrId("about-author-label")
                text: "Miklós Márton"
            }

            LabelText {
                //: About page label
                //% "Special thanks to"
                label: qsTrId("about-spec-thanks-label")
                //: About page text
                //% "<ahref='https://gist.github.com/scrool'>@scrool</a> for the Brandbase car protocol"
                text: qsTrId("about-spec-thanks-label-text")
            }
            Item {
                width: 1
                height: Theme.paddingLarge
            }
        }

        VerticalScrollDecorator { }
    }
}
