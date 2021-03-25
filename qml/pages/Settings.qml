import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView
        model: 20
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Nested Page")
        }
        delegate: DiscoveredRCItem {
            id: delegate

        }
        VerticalScrollDecorator {}
    }
}
