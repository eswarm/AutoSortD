import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    id: item1
    width: 640
    height: 480
    opacity: 1
    clip: false

    property alias button1: selectButton

    Rectangle {
        id: rectangle1
        color: "#e5e9fa"
        anchors.fill: parent
        Layout.fillHeight: true
        Layout.fillWidth: true
        border.width: 1

        ColumnLayout {
            id: layout
            anchors.fill: parent

            Label {
                id: appTitle
                color: "#170808"
                text: qsTr("Smart Folder")
                horizontalAlignment: Text.AlignHCenter
                anchors.top: parent.top
                anchors.topMargin: 5
                Layout.fillWidth: true
                font.pointSize: 40
                font.family: "Verdana"
            }

            RowLayout {
                id: folderLayout
                Layout.fillWidth: true;
                TextEdit {
                    id: textEdit1
                    width: 80
                    height: 20
                    text: qsTr("Select Folder")
                    font.pixelSize: 12
                }

                Button {
                    id: selectButton
                    text: qsTr("Select")
                }
            }
        }
    }
}
