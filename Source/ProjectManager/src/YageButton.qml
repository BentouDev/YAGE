import QtQuick 2.6
import QtQuick.Controls 2.0

Button {
    id: control
    property string image
    contentItem: Rectangle {
        color: "transparent"
        Image {
            id: icon
            
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            source: "image://standardIcon/" + image
            width: 20; height: 20;
            fillMode: Image.PreserveAspectFit
        }
        Text {
            id: label

            anchors.left: icon.right
            anchors.leftMargin: 10
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