import QtQuick 2.6
import QtQuick.Controls 1.0
import Yage.ProjectManager.Backend 1.0

Rectangle {
    color: "#202020"

    anchors.centerIn: parent
    anchors.fill: parent

    StackView {
        id: stack

        anchors.centerIn: parent
        anchors.fill: parent

        delegate: StackViewDelegate {
            function transitionFinished(properties)
            {
                properties.exitItem.opacity = 1
            }

            pushTransition: StackViewTransition {
                PropertyAnimation {
                    target: enterItem
                    property: "opacity"
                    from: 0
                    to: 1
                    duration: 100
                }
                PropertyAnimation {
                    target: exitItem
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: 100
                }
            }
        }

        Component {
            id: thePage

            Rectangle {
                
                color: "#202020"

                width: parent.width

                Rectangle {
                    id: project_setup

                    color: "#2f2f2f"
                    
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                    }

                    width: parent.width
                    height: 140

                    Text {
                        anchors.bottom: path_field.top
                        anchors.left: path_field.left
                        anchors.bottomMargin: 2
                        text: "Path"
                        color: "#ccc"
                    }

                    Item {
                        id: path_field
                        height: 26
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            top: parent.top
                            left: parent.left
                            right: parent.right
                            topMargin: 20
                            leftMargin: 64
                            rightMargin: 64
                        }

                        TextField {
                            id: path_field_form
                            text: backend.CurrentDir
                            anchors.left: parent.left
                            anchors.right: path_field_button.left
                            onTextChanged: backend.CurrentDir = text
                        }

                        Button {
                            id: path_field_button
                            text: "..."
                            onClicked: backend.OnPickFolder();
                            anchors.right: parent.right
                            anchors.top: path_field_form.top
                            anchors.bottom: path_field_form.bottom
                            width: 48
                        }
                    }
                    
                    Text {
                        anchors.bottom: name_field.top
                        anchors.left: name_field.left
                        anchors.bottomMargin: 2
                        text: "Name"
                        color: "#ccc"
                    }

                    TextField {
                        id: name_field

                        placeholderText: "Type project name..."
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            top: path_field.bottom
                            left: parent.left
                            right: parent.right
                            topMargin: 20
                            leftMargin: 64
                            rightMargin: 64
                        }
                    }
                }

                ScrollView {
                    id: list_scroll

                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        left: parent.left
                        right: parent.right
                        leftMargin: 64
                        rightMargin: 64
                    }

                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                        bottom: project_setup.top
                        bottomMargin: 24
                        topMargin: 24
                    }

                    ListView {
                        id: template_list

                        anchors.fill: parent
                        anchors.rightMargin: 10

                        interactive: false
                        focus: true

                        model: templates

                        highlight: Rectangle { color: "#444" }
                        delegate: Rectangle {
                            color: "transparent"

                            width: parent.width
                            height: 20
                            Text {
                                color: "#aaa"
                                width: parent.width
                                text: modelData
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: template_list.currentIndex = index
                            }
                        }
                    }
                }

                Text {
                    anchors.bottom: list_scroll.top
                    anchors.left: list_scroll.left
                    anchors.bottomMargin: 5
                    text: "Project Templates"
                    color: "#ccc"
                }

                YageButton {
                    width: 120; height: 42
                    
                    text: "Back "
                    image: "images/round_arrow_back_white_18dp.png"
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    onClicked: stack.pop()
                }
                
                YageButton {
                    width: 120; height: 42
                    
                    text: "Create"
                    image: "images/round_navigate_next_white_18dp.png"
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    onClicked: backend.OnNewProject(name_field.text, template_list.currentIndex)
                }
            }
        }

        initialItem: Rectangle {
            id: main

            color: "#202020"

            Component {
                id: listElement
                YageButton {

                    width: parent.width

                    isProject: true
                    image: "images/twotone_videogame_asset_white_18dp.png"
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

                ScrollView {
                    anchors.centerIn: parent
                    anchors.fill: parent

                    ListView {
                        id: listView

                        anchors.centerIn: parent
                        anchors.fill: parent
                        anchors.rightMargin: 10

                        model: backend.Projects
                        delegate: listElement

                        interactive: false
                    }
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
                            image: "images/twotone_create_new_folder_white_18dp.png"
                            onClicked: stack.push(thePage)
                        }
                        YageButton {
                            width: buttons.width
                            text: "Open project..."
                            image: "images/twotone_folder_open_white_18dp.png"
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
                            image: "images/twotone_cloud_download_white_18dp.png"
                            onClicked: backend.OnCheckUpdates();
                        }
                        YageButton {
                            width: buttons.width
                            text: "About"
                            image: "images/twotone_info_white_18dp.png"
                            onClicked: backend.OnAbout();
                        }
                    }
                }
            }
        }
    }
}