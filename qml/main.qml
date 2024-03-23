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
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border { 
                width: 1
                color: "blue"
            }
        }
        // MouseArea {
        //     anchors.fill: parent
        //     onClicked: {
        //         mapId.render()
        //     }
        // }
    }


}