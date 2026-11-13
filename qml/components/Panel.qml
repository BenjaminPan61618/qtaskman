import QtQuick
import org.kde.kirigami as Kirigami

Rectangle {
    id: root

    property int padding: 16
    default property alias contentData: inner.data

    color: Kirigami.Theme.alternateBackgroundColor
    radius: 12
    border.width: 1
    border.color: Qt.rgba(Kirigami.Theme.textColor.r, Kirigami.Theme.textColor.g,
                          Kirigami.Theme.textColor.b, 0.08)
    clip: true

    Item {
        id: inner
        anchors.fill: parent
        anchors.margins: root.padding
    }
}
