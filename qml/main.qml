import QtQuick
import QtQuick.Controls
import Map 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Map Viewer")

    Map {
        width: 200
        height: 200
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border { 
                width: 1
                color: "blue"
            }
        }
    }


}