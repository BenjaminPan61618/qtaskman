import QtQuick
import QtQuick.Controls as QQC2

Rectangle {
    id: root

    readonly property string message: label.text

    width: Math.min(560, label.implicitWidth + 36)
    height: 42
    radius: 9
    color: Qt.rgba(0.08, 0.08, 0.09, 0.92)
    opacity: 0
    visible: opacity > 0

    function show(text) {
        label.text = text
        root.opacity = 1
        hideTimer.restart()
    }

    QQC2.Label {
        id: label
        anchors.centerIn: parent
        width: parent.width - 32
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
        font.pointSize: 9
        color: "#f2f2f3"
    }

    Timer {
        id: hideTimer
        interval: 2800
        onTriggered: root.opacity = 0
    }

    Behavior on opacity {
        NumberAnimation { duration: 180 }
    }
}
