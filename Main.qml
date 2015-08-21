import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

import eswarm.DirWatcher 1.0

ApplicationWindow {

    title: qsTr("AutoSortD")
    width: 480
    height: 640
    color: "#2196f3"
    visible: true
    property int margin : 10


    ColumnLayout {
        id: mainColumn
        anchors.margins: margin
        anchors.fill : parent
        Layout.minimumHeight: 480
        Layout.minimumWidth: 640

        ColumnLayout {

            id: col
            anchors.horizontalCenter: parent.horizontalCenter
            Layout.fillWidth: true
            anchors.top: parent.top
            anchors.right: parent.right

            Label {
                color: "#ffffff"
                text : "Folders to auto sort"
                font.pointSize: 16
                anchors.horizontalCenter: parent.horizontalCenter
            }

            ColumnLayout {
            id : folderLayout
            }

            Button  {
                text : "Add folder"
                anchors.horizontalCenter: parent.horizontalCenter
                style: CustButtonStyle{}
                onClicked: fileDialog.open()
            }
        }

        Label {
            color: "#ffffff"
            text : "Changes Made"            
            font.pointSize: 16
            anchors.horizontalCenter: parent.horizontalCenter
        }

        TextArea {
            id: changesText
            Layout.fillHeight: true
            Layout.fillWidth: true
            anchors.margins: 10
            readOnly: true
        }

        Button  {
            text : "clear"
            anchors.horizontalCenter: parent.horizontalCenter
            style: CustButtonStyle {}
        }
    }

    Component.onCompleted: {
        //addFolderRow("a big path to somewhere", "name");
        var folders = DirWatcher.getFolders();
        //printObject(folders);
    }

    function addFolderRow(path) {
        var folderRowComp;
        var folderRowObj;
        folderRowComp = Qt.createComponent("FolderRow.qml")
        folderRowObj = folderRowComp.createObject(folderLayout, { "text" : path });
        if (folderRowObj === null) {
            // Error Handling
            console.log("Error creating object");
        }
    }

    function printObject(obj) {
    for (var p in obj) {
            if (obj.hasOwnProperty(p)) {
               console.log( p + '::' + obj[p] + '\n');
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a folder"
        folder: shortcuts.home
        selectFolder: true
        onAccepted: {
            var path = fileDialog.fileUrl.toString();
            // remove prefixed "file:///"
            path = path.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"");
            // unescape html codes like '%23' for '#'
            var cleanPath = decodeURIComponent(path);
            //Qt.quit()

            addFolderRow(cleanPath)
        }
        onRejected: {
            console.log("Canceled")
            //Qt.quit()
        }
       // Component.onCompleted: visible = true
    }
}
