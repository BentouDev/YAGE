import QtQuick 2.6
import QtQuick.Controls 2.0
import Yage.ProjectManager.Backend 1.0

Rectangle {
    id: rectangle
    color: "green"
	anchors.centerIn: parent
	anchors.fill: parent

    Text {
        id: text
        text: "This is QML code.\n(Click to pause)\n"
        font.pointSize: 14
        anchors.centerIn: parent
        PropertyAnimation {
            id: animation
            target: text
            property: "rotation"
            from: 0; to: 360; duration: 5000
            loops: Animation.Infinite
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: animation.paused ? animation.resume() : animation.pause()
    }
    Component.onCompleted: animation.start()
	
    ListModel {
        id: mock
        ListElement {
            Name: "Bill Smith"
            number: "555 3264"
        }
        ListElement {
            Name: "John Brown"
            number: "555 8426"
        }
        ListElement {
            Name: "Sam Wise"
            number: "555 0473"
        }
    }

    ListView {
	    anchors.centerIn: parent
        anchors.fill: parent

        focus: true
        model: projects // mock //backend.Project //

        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        delegate: Rectangle {
            height: 25
            width: 100
            color: "red"
            Text {
                text: Name
                color: "black"
            }
        }
    }
}