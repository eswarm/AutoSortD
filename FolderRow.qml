import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

RowLayout {

    id : folderRow;
    property alias text: folderLabel.text;
    signal remove(string text);

    Label {
        id : folderLabel
        color: "#ffffff"
        text: "Folder"
        font.pixelSize: 22
        font.bold: true
        Layout.fillWidth: true        
    }

    Button  {
        id : removeButton
        text : "Remove"
        //style : CustButtonStyle{}
        onClicked: remove(parent.text)
    }

}

