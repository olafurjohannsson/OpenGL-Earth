import QtQuick
import QtQuick.Controls
import Map 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Map Viewer")

    Map {
        width: parent.width
        height: parent.height
    }

    Rectangle {
        width: 100
        height: 100
        color: "red"
    }
}