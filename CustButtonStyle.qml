import QtQuick 2.0
import QtQuick.Controls.Styles 1.4

ButtonStyle {
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
