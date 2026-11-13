import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

RowLayout {
    id: root

    property var labels: []
    property int currentIndex: 0

    signal activated(int index)

    spacing: 6

    Repeater {
        model: root.labels

        delegate: Rectangle {
            id: chip

            required property int index
            required property string modelData

            implicitHeight: 30
            implicitWidth: text.implicitWidth + 26
            radius: 8
            color: root.currentIndex === index
                   ? Qt.rgba(Kirigami.Theme.highlightColor.r, Kirigami.Theme.highlightColor.g,
                             Kirigami.Theme.highlightColor.b, 0.16)
                   : (chipMouse.containsMouse
                      ? Qt.rgba(Kirigami.Theme.textColor.r, Kirigami.Theme.textColor.g,
                                Kirigami.Theme.textColor.b, 0.07)
                      : "transparent")
            border.width: 1
            border.color: root.currentIndex === index
                          ? Qt.rgba(Kirigami.Theme.highlightColor.r, Kirigami.Theme.highlightColor.g,
                                    Kirigami.Theme.highlightColor.b, 0.45)
                          : "transparent"

            QQC2.Label {
                id: text
                anchors.centerIn: parent
                text: chip.modelData
                font.pointSize: 9
                font.bold: root.currentIndex === chip.index
                color: root.currentIndex === chip.index
                       ? Kirigami.Theme.highlightColor
                       : Kirigami.Theme.textColor
                opacity: root.currentIndex === chip.index ? 1.0 : 0.78
            }

            MouseArea {
                id: chipMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: root.activated(chip.index)
            }
        }
    }
}
