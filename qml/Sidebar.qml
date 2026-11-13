import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import qtaskman 1.0
import "components"

Rectangle {
    id: root

    property int currentIndex: 0
    property int processCount: 0
    property int appCount: 0

    signal navigate(int index)
    signal openMonitor(int tab)
    signal openSettings()
    signal openAbout()

    property var navItems: (Translator.language, [
        { icon: "computer", label: qsTr("硬件总览"), badge: 0 },
        { icon: "window", label: qsTr("前台应用"), badge: root.appCount },
        { icon: "view-process-tree", label: qsTr("进程管理"), badge: root.processCount },
        { icon: "utilities-system-monitor", label: qsTr("硬件监控"), badge: 0 }
    ])

    color: Kirigami.Theme.alternateBackgroundColor

    Rectangle {
        width: 1
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: Qt.rgba(Kirigami.Theme.textColor.r, Kirigami.Theme.textColor.g, Kirigami.Theme.textColor.b, 0.08)
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 22
        anchors.bottomMargin: 18
        spacing: 4

        RowLayout {
            Layout.bottomMargin: 14
            spacing: 10

            Kirigami.Icon {
                source: "utilities-system-monitor"
                Layout.preferredWidth: 34
                Layout.preferredHeight: 34
            }

            ColumnLayout {
                spacing: 0

                QQC2.Label {
                    text: "qTaskMan"
                    font.pointSize: 15
                    font.bold: true
                    color: Kirigami.Theme.textColor
                }

                QQC2.Label {
                    text: qsTr("系统任务管理器")
                    font.pointSize: 8.5
                    color: Kirigami.Theme.textColor
                    opacity: 0.5
                }
            }

            Item { Layout.fillWidth: true }

            QQC2.Button {
                id: menuButton
                implicitWidth: 32
                implicitHeight: 32
                flat: true
                icon.source: "open-menu-symbolic"
                icon.width: 20
                icon.height: 20
                QQC2.ToolTip.text: qsTr("菜单")
                QQC2.ToolTip.visible: hovered
                QQC2.ToolTip.delay: 500

                QQC2.Menu {
                    id: appMenu
                    QQC2.MenuItem {
                        text: qsTr("设置")
                        icon.source: "preferences-system"
                        onTriggered: root.openSettings()
                    }
                    QQC2.MenuItem {
                        text: qsTr("关于")
                        icon.source: "help-about"
                        onTriggered: root.openAbout()
                    }
                }

                onClicked: appMenu.open()
            }
        }

        Repeater {
            model: root.navItems

            delegate: Rectangle {
                id: navItem

                required property int index
                required property var modelData

                Layout.fillWidth: true
                Layout.preferredHeight: 44
                radius: 8
                color: root.currentIndex === index
                       ? Qt.rgba(Kirigami.Theme.highlightColor.r, Kirigami.Theme.highlightColor.g,
                                 Kirigami.Theme.highlightColor.b, 0.16)
                       : (navMouse.containsMouse ? Qt.rgba(Kirigami.Theme.textColor.r, Kirigami.Theme.textColor.g,
                                                           Kirigami.Theme.textColor.b, 0.06)
                                                 : "transparent")

                Rectangle {
                    width: 3
                    height: 20
                    radius: 1.5
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    color: Kirigami.Theme.highlightColor
                    visible: root.currentIndex === parent.index
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 13
                    anchors.rightMargin: 12
                    spacing: 11

                    Kirigami.Icon {
                        source: navItem.modelData.icon
                        Layout.preferredWidth: 18
                        Layout.preferredHeight: 18
                        opacity: root.currentIndex === navItem.index ? 1.0 : 0.72
                    }

                    QQC2.Label {
                        text: navItem.modelData.label
                        Layout.fillWidth: true
                        font.pointSize: 10
                        font.bold: root.currentIndex === navItem.index
                        color: root.currentIndex === navItem.index
                               ? Kirigami.Theme.highlightColor
                               : Kirigami.Theme.textColor
                        opacity: root.currentIndex === navItem.index ? 1.0 : 0.85
                    }

                    QQC2.Label {
                        visible: navItem.modelData.badge > 0
                        text: navItem.modelData.badge
                        font.pointSize: 8
                        color: Kirigami.Theme.textColor
                        opacity: 0.45
                    }
                }

                MouseArea {
                    id: navMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.navigate(navItem.index)
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }

        SparklineCard {
            Layout.fillWidth: true
            title: qsTr("CPU")
            value: monitor.cpuPercent
            history: monitor.cpuHistory
            lineColor: Kirigami.Theme.positiveTextColor
            onClicked: root.openMonitor(0)
        }

        SparklineCard {
            Layout.fillWidth: true
            title: qsTr("内存")
            value: monitor.memPercent
            history: monitor.memHistory
            lineColor: Kirigami.Theme.highlightColor
            onClicked: root.openMonitor(1)
        }

        Item {
            Layout.preferredHeight: 10
        }

        QQC2.Label {
            Layout.fillWidth: true
            text: systemInfo.hostname + "\n" + systemInfo.kernel
            font.pointSize: 7.5
            color: Kirigami.Theme.textColor
            opacity: 0.4
            elide: Text.ElideRight
            wrapMode: Text.WrapAnywhere
            maximumLineCount: 2
            lineHeight: 1.2
        }
    }

    component SparklineCard: Rectangle {
        id: card

        property string title: ""
        property real value: 0
        property var history: []
        property color lineColor: Kirigami.Theme.highlightColor
        signal clicked()

        implicitHeight: 84
        radius: 10
        color: Kirigami.Theme.backgroundColor
        border.width: 1
        border.color: Qt.rgba(Kirigami.Theme.textColor.r, Kirigami.Theme.textColor.g,
                              Kirigami.Theme.textColor.b, cardMouse.containsMouse ? 0.2 : 0.08)

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 11
            spacing: 2

            RowLayout {
                spacing: 6

                QQC2.Label {
                    text: card.title
                    font.pointSize: 8.5
                    font.bold: true
                    color: Kirigami.Theme.textColor
                    opacity: 0.7
                }

                Item {
                    Layout.fillWidth: true
                }

                QQC2.Label {
                    text: card.value.toFixed(0) + "%"
                    font.pointSize: 10
                    font.bold: true
                    color: card.lineColor
                }
            }

            Sparkline {
                Layout.fillWidth: true
                Layout.fillHeight: true
                values: card.history
                lineColor: card.lineColor
                maxValue: 100
            }
        }

        MouseArea {
            id: cardMouse
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: card.clicked()
        }
    }
}
