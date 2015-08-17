import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    title: qsTr("AutoSortD")
    width: 640
    height: 480
    color: "#2196f3"
    visible: true
    property int margin : 10

    /*
    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: messageDialog.show(qsTr("Open action triggered"));
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }
    */

    ColumnLayout {
        id: mainColumn
        anchors.margins: margin
        anchors.fill : parent


        ColumnLayout {
            id: col
            Layout.fillWidth: true
            anchors.top: parent.top
            anchors.right: parent.right

            Label {
                color: "#ffffff"
                    text : "Folders to auto sort"
                    font.pointSize: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                FolderRow {}

                Button  {
                    text : "Add folder"                    
                    anchors.horizontalCenter: parent.horizontalCenter

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

        Label {
            color: "#ffffff"
            text : "Changes Made"
            font.pointSize: 16
            anchors.horizontalCenter: parent.horizontalCenter
        }

        TextArea {
            id: textArea1
            Layout.fillHeight: true
            Layout.fillWidth: true
            anchors.margins: 10
        }

        Button  {
            text : "clear"
            anchors.horizontalCenter: parent.horizontalCenter

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


    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }


}
