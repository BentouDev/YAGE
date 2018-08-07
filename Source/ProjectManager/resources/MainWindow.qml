import QtQuick 2.6
import QtQuick.Controls 1.0
import Yage.ProjectManager.Backend 1.0

Rectangle {
    id: main

	anchors.centerIn: parent
	anchors.fill: parent

    color: "#202020"

    Component {
        id: listElement
        YageButton {

            width: parent.width

            isProject: true
            image: "SP_DirIcon"
            text: model.Name
            desc: model.Path
            date: model.Date

            onClicked: modelData.OnOpen();
        }
    }

    Rectangle {
        anchors.centerIn: parent
        anchors.fill: parent

        anchors.leftMargin: 200
        width: parent.width - 200

        color: "#2f2f2f"

        ListView {
            id: listView

            anchors.centerIn: parent
            anchors.fill: parent

            model: backend.Projects
            delegate: listElement

            interactive: false
        }
    }

    
    Rectangle {
        id: buttons

        anchors.centerIn: parent
        anchors.fill: parent

        anchors.rightMargin: parent.width - 200
        width: 200

        color: "transparent"

        Row {
            Column {
                YageButton {
                    width: buttons.width
                    text: "New project"
                    image: "SP_FileIcon"
                    onClicked: backend.OnNewProject();
                }
                YageButton {
                    width: buttons.width
                    text: "Open project..."
                    image: "SP_DirOpenIcon"
                    onClicked: backend.OnOpenProject();
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
                    onClicked: backend.OnCheckUpdates();
                }
                YageButton {
                    width: buttons.width
                    text: "About"
                    image: "SP_MessageBoxQuestion"
                    onClicked: backend.OnAbout();
                }
            }
        }
    }
}