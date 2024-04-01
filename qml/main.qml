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
            color: "red"
            width: 10
            height: 10
            x: mapId.centerX
            y: mapId.centerY
        }

        Text {
            id: cursorCoordinateId
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.leftMargin: 8
            text: "Cursor LonLat: " + mapId.cursorCoordinate
            color: "white"
        }
        Text {
            id: centerCoordinateId
            anchors.left: parent.left
            anchors.top: cursorCoordinateId.bottom
            anchors.topMargin: 8
            anchors.leftMargin: 8
            text: "Center: " + mapId.centerX + ", " + mapId.centerY
            color: "white"
        }
        Text {
            anchors.top: centerCoordinateId.bottom
            anchors.left: parent.left
            anchors.topMargin: 8
            anchors.leftMargin: 8
            text: "Zoom: " + mapId.zoom
            color: "white"
        }
    }


}