import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import qtaskman 1.0
import "../components"

Item {
    id: page
    property var appWindow
    property int highlightPid: -1

    property int colUser: 92
    property int colCpu: 70
    property int colMem: 100
    property int colNet: 122
    property int colDisk: 122

    property real nameWidth: Math.max(160, processList.width - 44
                                      - (colUser + colCpu + colMem + colNet + colDisk) - 5 * 12)

    function focusPid(pid) {
        const row = processModel.focusPid(pid)
        if (row >= 0) {
            highlightPid = pid
            highlightTimer.restart()
            processList.positionViewAtIndex(row, ListView.Center)
        } else if (appWindow) {
            appWindow.notify(qsTr("进程 %1 已不存在").arg(pid))
        }
    }

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
        spacing: 14

        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            ColumnLayout {
                spacing: 2

                QQC2.Label {
                    text: qsTr("进程管理")
                    font.pointSize: 19
                    font.bold: true
                    color: Kirigami.Theme.textColor
                }

                QQC2.Label {
                    text: processModel.flatMode
                          ? qsTr("扁平列表 · 右键可发送信号、设置优先级或结束进程")
                          : qsTr("树状进程树 · 右键可发送信号、设置优先级或结束进程")
                    font.pointSize: 9
                    color: Kirigami.Theme.textColor
                    opacity: 0.5
                }
            }

            Item {
                Layout.fillWidth: true
            }

            QQC2.Label {
                text: qsTr("%1 行").arg(processModel.count)
                font.pointSize: 8.5
                color: Kirigami.Theme.textColor
                opacity: 0.45
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            TabBar {
                labels: (Translator.language, [qsTr("个人进程"), qsTr("用户进程"), qsTr("系统进程"), qsTr("所有进程")])
                currentIndex: processModel.filterMode
                onActivated: function(index) {
                    processModel.filterMode = index
                }
            }

            Item {
                Layout.fillWidth: true
            }

            QQC2.ToolButton {
                text: processModel.flatMode ? qsTr("树形视图") : qsTr("列表视图")
                icon.name: processModel.flatMode ? "view-list-tree" : "view-list-details"
                font.pointSize: 8.5
                onClicked: processModel.flatMode = !processModel.flatMode
            }

            QQC2.ToolButton {
                text: qsTr("展开全部")
                visible: !processModel.flatMode
                font.pointSize: 8.5
                onClicked: processModel.expandAll()
            }

            QQC2.ToolButton {
                text: qsTr("折叠全部")
                visible: !processModel.flatMode
                font.pointSize: 8.5
                onClicked: processModel.collapseAll()
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
                    Layout.preferredHeight: 34

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 16
                        anchors.rightMargin: 28
                        spacing: 12

                        QQC2.Label {
                            Layout.preferredWidth: page.nameWidth
                            text: qsTr("名称")
                            font.pointSize: 8.5
                            color: Kirigami.Theme.textColor
                            opacity: 0.5
                        }
                        QQC2.Label {
                            Layout.preferredWidth: page.colUser
                            text: qsTr("用户")
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
                    id: processList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: processModel
                    boundsBehavior: Flickable.StopAtBounds

                    QQC2.ScrollBar.vertical: QQC2.ScrollBar {}

                    delegate: Rectangle {
                        id: procRow

                        required property int index
                        required property int pid
                        required property string name
                        required property string command
                        required property string user
                        required property real cpu
                        required property real mem
                        required property real memPercent
                        required property real netUp
                        required property real netDown
                        required property real diskRead
                        required property real diskWrite
                        required property string iconName
                        required property int depth
                        required property bool hasChildren
                        required property bool expanded
                        required property bool isSelf
                        required property string state
                        required property bool isNew

                        width: processList.width
                        height: 34
                        color: page.highlightPid === pid
                               ? Qt.rgba(Kirigami.Theme.highlightColor.r, Kirigami.Theme.highlightColor.g,
                                         Kirigami.Theme.highlightColor.b, 0.16)
                               : page.rowColor(procRow.state, procRow.isNew,
                                               procMouse.containsMouse, index % 2 === 1)

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 16
                            anchors.rightMargin: 28
                            spacing: 12

                            RowLayout {
                                Layout.preferredWidth: page.nameWidth
                                spacing: 6

                                Item {
                                    Layout.preferredWidth: procRow.depth * 14
                                    Layout.preferredHeight: 1
                                    visible: !processModel.flatMode
                                }

                                Item {
                                    Layout.preferredWidth: 16
                                    Layout.preferredHeight: 16
                                    visible: !processModel.flatMode

                                    Kirigami.Icon {
                                        anchors.centerIn: parent
                                        width: 11
                                        height: 11
                                        source: procRow.expanded ? "arrow-down" : "arrow-right"
                                        visible: procRow.hasChildren
                                        opacity: 0.6
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        enabled: procRow.hasChildren
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: processModel.toggleExpanded(index)
                                    }
                                }

                                Kirigami.Icon {
                                    Layout.preferredWidth: 18
                                    Layout.preferredHeight: 18
                                    source: procRow.iconName.length > 0 ? procRow.iconName : "application-x-executable"
                                    fallback: "application-x-executable"
                                }

                                QQC2.Label {
                                    Layout.fillWidth: true
                                    text: procRow.name
                                    font.pointSize: 9
                                    color: Kirigami.Theme.textColor
                                    elide: Text.ElideRight
                                }

                                QQC2.Label {
                                    visible: procRow.isSelf
                                    text: qsTr("本应用")
                                    font.pointSize: 7
                                    color: Kirigami.Theme.highlightColor
                                    opacity: 0.9
                                }
                            }

                            QQC2.Label {
                                Layout.preferredWidth: page.colUser
                                text: procRow.user
                                font.pointSize: 8.5
                                color: Kirigami.Theme.textColor
                                opacity: 0.6
                                elide: Text.ElideRight
                            }

                            QQC2.Label {
                                Layout.preferredWidth: page.colCpu
                                horizontalAlignment: Text.AlignRight
                                text: procRow.cpu.toFixed(1) + "%"
                                font.pointSize: 8.5
                                color: procRow.cpu > 50 ? Kirigami.Theme.negativeTextColor : Kirigami.Theme.textColor
                                opacity: 0.9
                            }

                            ColumnLayout {
                                Layout.preferredWidth: page.colMem
                                spacing: 0

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: monitor.formatBytes(procRow.mem)
                                    font.pointSize: 8.5
                                    color: Kirigami.Theme.textColor
                                    opacity: 0.85
                                }

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: monitor.formatPercent(procRow.memPercent, 1)
                                    font.pointSize: 7
                                    color: Kirigami.Theme.textColor
                                    opacity: 0.38
                                }
                            }

                            ColumnLayout {
                                Layout.preferredWidth: page.colNet
                                spacing: 0

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: qsTr("↑ %1").arg(monitor.formatRate(procRow.netUp))
                                    font.pointSize: 7.5
                                    color: Kirigami.Theme.positiveTextColor
                                    opacity: 0.8
                                }

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: qsTr("↓ %1").arg(monitor.formatRate(procRow.netDown))
                                    font.pointSize: 7.5
                                    color: Kirigami.Theme.highlightColor
                                    opacity: 0.8
                                }
                            }

                            ColumnLayout {
                                Layout.preferredWidth: page.colDisk
                                spacing: 0

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: qsTr("读 %1").arg(monitor.formatRate(procRow.diskRead))
                                    font.pointSize: 7.5
                                    color: Kirigami.Theme.textColor
                                    opacity: 0.6
                                }

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: qsTr("写 %1").arg(monitor.formatRate(procRow.diskWrite))
                                    font.pointSize: 7.5
                                    color: Kirigami.Theme.textColor
                                    opacity: 0.6
                                }
                            }
                        }

                        MouseArea {
                            id: procMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            acceptedButtons: Qt.RightButton

                            QQC2.ToolTip.visible: containsMouse
                            QQC2.ToolTip.delay: 800
                            QQC2.ToolTip.text: qsTr("PID %1\n%2").arg(procRow.pid).arg(procRow.command.length > 0 ? procRow.command : procRow.name)

                            onClicked: {
                                contextMenu.pid = procRow.pid
                                contextMenu.processName = procRow.name
                                contextMenu.popup()
                            }
                        }
                    }
                }
            }
        }
    }

    Timer {
        id: highlightTimer
        interval: 3000
        onTriggered: page.highlightPid = -1
    }

    ProcessContextMenu {
        id: contextMenu
        appWindow: page.appWindow
    }
}
