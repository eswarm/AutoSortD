import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

ApplicationWindow {
    title: qsTr("AutoSortD")
    width: 640
    height: 480
    visible: true

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

    Rectangle {
        anchors.fill: parent
        color : blue;

        GroupBox {
            anchors.fill: parent
            title: "Monitored folders"
            ColumnLayout {



                Button  {
                    text : "Add folder"
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

    FolderRow {
    RowLayout {

        Label {

            text: "Folder"
            font.pixelSize: 22
            font.bold: true
            color: "steelblue"
        }

        Button  {
            text : "Remove"
        }
    }
    }

}
