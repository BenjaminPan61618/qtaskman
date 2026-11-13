import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import "../components"

Item {
    id: page
    property var appWindow

    property int colCpu: 74
    property int colMem: 104
    property int colNet: 128
    property int colDisk: 128
    property int colPower: 64

    property real nameWidth: Math.max(160, appsList.width - 44
                                      - (colCpu + colMem + colNet + colDisk + colPower) - 5 * 12)

    function rowColor(state, isNew, hovered, striped) {
        if (state === "Z" || state === "X") {
            return Qt.rgba(0.55, 0.55, 0.60, 0.14)
        }
        if (state === "T" || state === "t") {
            return Qt.rgba(0.95, 0.72, 0.20, 0.16)
        }
        if (state === "D") {
            return Qt.rgba(0.93, 0.36, 0.36, 0.15)
        }
        if (isNew) {
            return Qt.rgba(0.35, 0.78, 0.45, 0.13)
        }
        if (hovered) {
            return Qt.rgba(Kirigami.Theme.highlightColor.r, Kirigami.Theme.highlightColor.g,
                           Kirigami.Theme.highlightColor.b, 0.07)
        }
        return striped
               ? Qt.rgba(Kirigami.Theme.textColor.r, Kirigami.Theme.textColor.g,
                         Kirigami.Theme.textColor.b, 0.025)
               : "transparent"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 16

        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 2

                QQC2.Label {
                    text: qsTr("前台应用")
                    font.pointSize: 19
                    font.bold: true
                    color: Kirigami.Theme.textColor
                }

                QQC2.Label {
                    text: qsTr("正在运行并拥有窗口的用户应用 · 每秒刷新")
                    font.pointSize: 9
                    color: Kirigami.Theme.textColor
                    opacity: 0.5
                }
            }

            Item {
                Layout.fillWidth: true
            }

            Rectangle {
                radius: 8
                implicitHeight: 28
                implicitWidth: countLabel.implicitWidth + 22
                color: Qt.rgba(Kirigami.Theme.textColor.r, Kirigami.Theme.textColor.g,
                               Kirigami.Theme.textColor.b, 0.06)

                QQC2.Label {
                    id: countLabel
                    anchors.centerIn: parent
                    text: qsTr("%1 个应用").arg(appModel.count)
                    font.pointSize: 8.5
                    color: Kirigami.Theme.textColor
                    opacity: 0.7
                }
            }
        }

        Panel {
            Layout.fillWidth: true
            Layout.fillHeight: true
            padding: 0

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 36

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 16
                        anchors.rightMargin: 28
                        spacing: 12

                        QQC2.Label {
                            Layout.preferredWidth: page.nameWidth
                            text: qsTr("应用")
                            font.pointSize: 8.5
                            color: Kirigami.Theme.textColor
                            opacity: 0.5
                        }
                        QQC2.Label {
                            Layout.preferredWidth: page.colCpu
                            horizontalAlignment: Text.AlignRight
                            text: "CPU"
                            font.pointSize: 8.5
                            color: Kirigami.Theme.textColor
                            opacity: 0.5
                        }
                        QQC2.Label {
                            Layout.preferredWidth: page.colMem
                            horizontalAlignment: Text.AlignRight
                            text: qsTr("内存")
                            font.pointSize: 8.5
                            color: Kirigami.Theme.textColor
                            opacity: 0.5
                        }
                        QQC2.Label {
                            Layout.preferredWidth: page.colNet
                            horizontalAlignment: Text.AlignRight
                            text: qsTr("网络 上传 / 下载")
                            font.pointSize: 8.5
                            color: Kirigami.Theme.textColor
                            opacity: 0.5
                        }
                        QQC2.Label {
                            Layout.preferredWidth: page.colDisk
                            horizontalAlignment: Text.AlignRight
                            text: qsTr("磁盘 读 / 写")
                            font.pointSize: 8.5
                            color: Kirigami.Theme.textColor
                            opacity: 0.5
                        }
                        QQC2.Label {
                            Layout.preferredWidth: page.colPower
                            horizontalAlignment: Text.AlignRight
                            text: qsTr("耗电")
                            font.pointSize: 8.5
                            color: Kirigami.Theme.textColor
                            opacity: 0.5
                        }
                    }

                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: 1
                        color: Qt.rgba(Kirigami.Theme.textColor.r, Kirigami.Theme.textColor.g,
                                       Kirigami.Theme.textColor.b, 0.08)
                    }
                }

                ListView {
                    id: appsList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: appModel
                    boundsBehavior: Flickable.StopAtBounds

                    QQC2.ScrollBar.vertical: QQC2.ScrollBar {}

                    delegate: Rectangle {
                        id: appRow

                        required property int index
                        required property int pid
                        required property string appName
                        required property string title
                        required property string iconName
                        required property real cpu
                        required property real mem
                        required property real memPercent
                        required property real netUp
                        required property real netDown
                        required property real diskRead
                        required property real diskWrite
                        required property bool activeWindow
                        required property string state
                        required property bool isNew

                        width: appsList.width
                        height: 58
                        color: page.rowColor(appRow.state, appRow.isNew,
                                             appMouse.containsMouse, index % 2 === 1)

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 16
                            anchors.rightMargin: 28
                            spacing: 12

                            RowLayout {
                                Layout.preferredWidth: page.nameWidth
                                spacing: 10

                                Kirigami.Icon {
                                    Layout.preferredWidth: 28
                                    Layout.preferredHeight: 28
                                    source: appRow.iconName.length > 0 ? appRow.iconName : "application-x-executable"
                                    fallback: "application-x-executable"
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 2

                                    RowLayout {
                                        spacing: 6

                                        QQC2.Label {
                                            text: appRow.appName
                                            font.pointSize: 9.5
                                            font.bold: true
                                            color: Kirigami.Theme.textColor
                                            elide: Text.ElideRight
                                            Layout.maximumWidth: 260
                                        }

                                        Rectangle {
                                            visible: appRow.activeWindow
                                            width: 6
                                            height: 6
                                            radius: 3
                                            color: Kirigami.Theme.positiveTextColor
                                        }
                                    }

                                    QQC2.Label {
                                        text: appRow.title.length > 0 ? appRow.title : qsTr("无窗口标题")
                                        font.pointSize: 8
                                        color: Kirigami.Theme.textColor
                                        opacity: 0.48
                                        elide: Text.ElideRight
                                        Layout.fillWidth: true
                                    }
                                }
                            }

                            QQC2.Label {
                                Layout.preferredWidth: page.colCpu
                                horizontalAlignment: Text.AlignRight
                                text: appRow.cpu.toFixed(1) + "%"
                                font.pointSize: 9
                                color: appRow.cpu > 50 ? Kirigami.Theme.negativeTextColor : Kirigami.Theme.textColor
                                opacity: 0.9
                            }

                            ColumnLayout {
                                Layout.preferredWidth: page.colMem
                                spacing: 1

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: monitor.formatBytes(appRow.mem)
                                    font.pointSize: 9
                                    color: Kirigami.Theme.textColor
                                    opacity: 0.9
                                }

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: monitor.formatPercent(appRow.memPercent, 1)
                                    font.pointSize: 7.5
                                    color: Kirigami.Theme.textColor
                                    opacity: 0.4
                                }
                            }

                            ColumnLayout {
                                Layout.preferredWidth: page.colNet
                                spacing: 1

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: qsTr("↑ %1").arg(monitor.formatRate(appRow.netUp))
                                    font.pointSize: 8
                                    color: Kirigami.Theme.positiveTextColor
                                    opacity: 0.85
                                }

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: qsTr("↓ %1").arg(monitor.formatRate(appRow.netDown))
                                    font.pointSize: 8
                                    color: Kirigami.Theme.highlightColor
                                    opacity: 0.85
                                }
                            }

                            ColumnLayout {
                                Layout.preferredWidth: page.colDisk
                                spacing: 1

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: qsTr("读 %1").arg(monitor.formatRate(appRow.diskRead))
                                    font.pointSize: 8
                                    color: Kirigami.Theme.textColor
                                    opacity: 0.65
                                }

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: qsTr("写 %1").arg(monitor.formatRate(appRow.diskWrite))
                                    font.pointSize: 8
                                    color: Kirigami.Theme.textColor
                                    opacity: 0.65
                                }
                            }

                            QQC2.Label {
                                Layout.preferredWidth: page.colPower
                                horizontalAlignment: Text.AlignRight
                                text: "—"
                                font.pointSize: 9
                                color: Kirigami.Theme.textColor
                                opacity: 0.35
                            }
                        }

                        MouseArea {
                            id: appMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            acceptedButtons: Qt.RightButton

                            QQC2.ToolTip.visible: containsMouse
                            QQC2.ToolTip.delay: 700
                            QQC2.ToolTip.text: qsTr("PID %1\n%2").arg(appRow.pid).arg(appRow.title)

                            onClicked: {
                                contextMenu.pid = appRow.pid
                                contextMenu.processName = appRow.appName
                                contextMenu.popup()
                            }
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    visible: appModel.count === 0

                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: 8

                        Kirigami.Icon {
                            Layout.alignment: Qt.AlignHCenter
                            source: "window"
                            width: 44
                            height: 44
                            opacity: 0.35
                        }

                        QQC2.Label {
                            text: qsTr("没有检测到前台窗口")
                            font.pointSize: 10
                            color: Kirigami.Theme.textColor
                            opacity: 0.55
                        }

                        QQC2.Label {
                            text: qsTr("请确认当前会话运行在 KDE Plasma / KWin 下")
                            font.pointSize: 8.5
                            color: Kirigami.Theme.textColor
                            opacity: 0.35
                        }
                    }
                }
            }
        }
    }

    ProcessContextMenu {
        id: contextMenu
        appWindow: page.appWindow
    }
}
