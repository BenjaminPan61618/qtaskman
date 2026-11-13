import QtQuick
import org.kde.kirigami as Kirigami

Rectangle {
    id: root

    property real value: 0
    property color fillColor: Kirigami.Theme.highlightColor
    property real trackHeight: 6

    implicitHeight: trackHeight
    height: trackHeight
    radius: height / 2
    color: Qt.rgba(Kirigami.Theme.textColor.r, Kirigami.Theme.textColor.g,
                   Kirigami.Theme.textColor.b, 0.10)

    Rectangle {
        width: Math.max(0, Math.min(1, root.value / 100)) * parent.width
        height: parent.height
        radius: parent.radius
        color: root.fillColor

        Behavior on width {
            NumberAnimation { duration: 220; easing.type: Easing.OutCubic }
        }
    }
}
