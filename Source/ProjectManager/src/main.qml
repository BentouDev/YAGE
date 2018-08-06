import QtQuick 2.6
import QtQuick.Controls 2.0
import Yage.ProjectManager.Backend 1.0

Rectangle {
    id: main
	anchors.centerIn: parent
	anchors.fill: parent

    Component {
        id: listElement
        Item {
            x: 10
            y: 10
            height: 36
            width: parent.width
            Row {                
                anchors.left: undefined
                anchors.right: parent.right
                Text {
                    text: model.Date
                    color: "black"
                }
            }
            Column {
                Text {
                    text: model.Name
                    color: "black"
                    font.pointSize: 10
                    elide: Text.ElideRight
                }
                Text {
                    text: model.Path
                    color: "black"
                    elide: Text.ElideRight
                }
            }
            MouseArea {                    
                z: 1
                anchors.fill: parent
                onClicked: listView.currentIndex = index
            }
        }
    }

    Rectangle {
        anchors.centerIn: parent
        anchors.fill: parent

        anchors.leftMargin: parent.width * 0.25
        width: parent.width * 0.75

        ListView {
            id: listView
            anchors.centerIn: parent
            anchors.fill: parent

            model: backend.Projects

            interactive: false
            focus: true
            
            onCurrentItemChanged: console.log((listView.currentIndex) + ' selected')
            
            highlight: Rectangle { color: "lightsteelblue"; }
            delegate: listElement
        }
    }

    
    Rectangle {
        id: buttons

        anchors.centerIn: parent
        anchors.fill: parent

        anchors.rightMargin: parent.width * 0.75
        width: parent.width * 0.25

        color: "gray"

        Row {
            Column {
                YageButton {
                    width: buttons.width
                    text: "New project"
                    image: "SP_FileIcon"
                }
                YageButton {
                    width: buttons.width
                    text: "Open project..."
                    image: "SP_DirOpenIcon"
                }
            }
        }
        Row {
            anchors.bottom: buttons.bottom
            Column {
                YageButton {
                    width: buttons.width
                    text: "Check for updates"
                    image: "SP_BrowserReload"
                }
                YageButton {
                    width: buttons.width
                    text: "About"
                    image: "SP_MessageBoxQuestion"
                }
            }
        }
    }
}