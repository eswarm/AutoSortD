import QtQuick 2.0

Rectangle {
    width: 100
    height: 62
    id: simpleButton


    property color buttonColor: "lightblue"
    property color onHoverColor: "gold"
    property color borderColor: "white"
    signal buttonClick()

    Text {
            id: buttonLabel
            anchors.centerIn: parent
            text: "Button"
        }

    onButtonClick: {
        console.log(buttonLabel.text + " clicked")
    }

    MouseArea{
        id:buttonMouseArea;
        onClicked: buttonClick()
        hoverEnabled: true
        onEntered: parent.border.color = onHoverColor
        onExited:  parent.border.color = borderColor
        anchors.fill: parent;
    }

    // Determines the color of the button by using the conditional operator
    color: buttonMouseArea.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
}


