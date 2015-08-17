import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

RowLayout {

    Label {
        color: "#ffffff"
        text: "Folder"
        font.pixelSize: 22
        font.bold: true
        Layout.fillWidth: true
    }

    Button  {
        text : "Remove"
        style: ButtonStyle {
               background: Rectangle {
                   implicitWidth: 100
                   implicitHeight: 25
                   border.width: control.activeFocus ? 2 : 1
                   radius: 4
                   gradient: Gradient {
                       GradientStop { position: 0 ; color: control.pressed ? "#2196f3" : "#42a5f5" }
                       GradientStop { position: 1 ; color: control.pressed ? "#2196f3" : "#42a5f5" }
                   }
               }
           }
    }
}

