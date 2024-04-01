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
        id: zoomId
        anchors.top: centerCoordinateId.bottom
        anchors.left: parent.left
        anchors.topMargin: 8
        anchors.leftMargin: 8
        text: "Zoom: " + mapId.zoom
        color: "white"
    }

    Item {
        width:400;
        height: 400;
        anchors.top: zoomId.bottom;
        anchors.left: parent.left;
        anchors.topMargin: 8
        anchors.leftMargin: 8

        Rectangle {
            id:comboBox
            property variant items: ["Orthographic", "Stereographic", "Mercator"];
            property alias selectedItem: chosenItemText.text;
            property alias selectedIndex: listView.currentIndex;
            signal comboClicked;
            width: 160;
            height: 30;
            z: 100;
            smooth:true;

            Rectangle {
                id:chosenItem
                radius:4;
                width:parent.width;
                height:comboBox.height;
                color: "lightsteelblue"
                smooth:true;
                Text {
                    anchors.top: parent.top;
                    anchors.left: parent.left;
                    anchors.margins: 8;
                    id:chosenItemText
                    text:comboBox.items[0];
                    font.family: "Arial"
                    font.pointSize: 14;
                    smooth:true
                }

                MouseArea {
                    anchors.fill: parent;
                    onClicked: comboBox.state = comboBox.state === "dropDown" ? "" : "dropDown"
                }
            }

            Rectangle {
                id:dropDown
                width:comboBox.width;
                height:0;
                clip:true;
                radius:4;
                anchors.top: chosenItem.bottom;
                anchors.margins: 2;
                color: "lightgray"
                ListView {
                    id:listView
                    height:400;
                    model: comboBox.items
                    currentIndex: 2
                    delegate: Item {
                        width:comboBox.width;
                        height: comboBox.height;
                        Text {
                            text: modelData
                            anchors.top: parent.top;
                            anchors.left: parent.left;
                            anchors.margins: 5;
                        }
                        MouseArea {
                            anchors.fill: parent;
                            onClicked: {
                                comboBox.state = ""
                                var prevSelection = chosenItemText.text
                                chosenItemText.text = modelData
                                if (chosenItemText.text != prevSelection)
                                {
                                    comboBox.comboClicked();
                                    mapId.setProjection(chosenItemText.text)
                                }
                                listView.currentIndex = index;
                            }
                        }
                    }
                }
            }
            Component {
                id: highlight
                Rectangle {
                    width:comboBox.width;
                    height:comboBox.height;
                    color: "red";
                    radius: 4
                }
            }
            states: State {
                name: "dropDown";
                PropertyChanges { target: dropDown; height: 40 * comboBox.items.length }
            }
            transitions: Transition {
                NumberAnimation { target: dropDown; properties: "height"; easing.type: Easing.OutExpo; duration: 400 }
            }
        }
    }
}