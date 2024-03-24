import QtQuick
import QtQuick.Controls
import Map 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 960
    title: qsTr("Map Viewer")

    Map {
        id: mapId
        width: parent.width
        height: parent.height
    }


}