import QtQuick 2.6
import QtQuick.Controls 2.0
import Yage.ProjectManager.Backend 1.0

Rectangle {
    id: rectangle
    color: "green"
	anchors.centerIn: parent
	anchors.fill: parent

	Backend {
		id: backend
	}

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
	
	TextField {
        text: backend.userName
        placeholderText: qsTr("User name")
        anchors.centerIn: parent

        onTextChanged: backend.userName = text
    }
}