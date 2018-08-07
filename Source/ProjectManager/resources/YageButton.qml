import QtQuick 2.6
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.4

Button {
    id: control
    property string image
    property string desc
    property string date
    property bool isProject

    style: ButtonStyle { 
        background: yage_button_background
        label: Loader {
            sourceComponent: control.isProject ? yage_button_project_entry : yage_button_single_line
        }
    }

    Component {
        id: yage_button_background
        Rectangle {
            implicitHeight: 42
            color: "transparent"
        }
    }

    Component {
        id: yage_button_project_entry
        Rectangle {
            color: control.pressed ? "#66000000" : (control.hovered ? "#22ffffff" : "transparent")
            Image {
                id: icon
                
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                source: "image://standardIcon/" + image
                width: 24; height: 24;

                fillMode: Image.PreserveAspectFit
            }
            Row {
                anchors.left: undefined
                anchors.right: parent.right
                Text {
                    text: control.date
                    color: "#aaa"
                }
            }
            Column {
                anchors.leftMargin: 10
                anchors.left: icon.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                Text {
                    text: control.text
                    color: "white"
                    font.pointSize: 10
                    elide: Text.ElideRight
                }
                Text {
                    text: control.desc
                    color: "#aaa"
                    elide: Text.ElideRight
                }
            }
        }
    }

    Component {
        id: yage_button_single_line
        Rectangle {
            color: control.pressed ? "#66000000" : (control.hovered ? "#22ffffff" : "transparent")
            Image {
                id: icon
                
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                source: "image://standardIcon/" + image
                width: 24; height: 24;

                fillMode: Image.PreserveAspectFit
            }
            Text {
                id: label

                color: "#aaa"

                anchors.leftMargin: 10
                anchors.left: icon.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                text: control.text
                horizontalAlignment: Text.AlignHLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight

                font.pointSize: 10
                font.underline: control.hovered        
            }
        }
    }
}