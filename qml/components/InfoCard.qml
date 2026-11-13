import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import "."

Panel {
    id: card

    property string icon: ""
    property string label: ""
    property string value: ""
    property string detail: ""
    property color tint: Kirigami.Theme.highlightColor

    implicitHeight: 104

    RowLayout {
        anchors.fill: parent
        spacing: 14

        Rectangle {
            Layout.alignment: Qt.AlignTop
            Layout.preferredWidth: 44
            Layout.preferredHeight: 44
            radius: 10
            color: Qt.rgba(card.tint.r, card.tint.g, card.tint.b, 0.14)

            Kirigami.Icon {
                anchors.centerIn: parent
                width: 22
                height: 22
                source: card.icon
                color: card.tint
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 3

            QQC2.Label {
                text: card.label
                font.pointSize: 8.5
                color: Kirigami.Theme.textColor
                opacity: 0.55
            }

            QQC2.Label {
                text: card.value
                font.pointSize: 11.5
                font.bold: true
                color: Kirigami.Theme.textColor
                wrapMode: Text.Wrap
                maximumLineCount: 2
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Item {
                Layout.fillHeight: true
            }

            QQC2.Label {
                visible: text.length > 0
                text: card.detail
                font.pointSize: 8
                color: Kirigami.Theme.textColor
                opacity: 0.45
                elide: Text.ElideRight
                Layout.fillWidth: true
            }
        }
    }
}
