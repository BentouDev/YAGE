import QtQuick 2.6
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.4

Button {
    id: control
    property string image
    style: ButtonStyle { 
        background: Rectangle {
            implicitHeight: 42
            color: "transparent"
        }
        label: Rectangle {
            color: control.hovered ? "#303030" : "transparent"
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