import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import qtaskman 1.0
import "../components"

Item {
    id: page
    property var appWindow
    property int currentTab: 0

    function setTab(index) {
        currentTab = index
    }

    function tempText(value) {
        if (value < 0) return qsTr("不可用")
        return appSettings.toDisplayTemp(value).toFixed(1) + " " + appSettings.tempUnitSymbol()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 14

        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 2

                QQC2.Label {
                    text: qsTr("硬件监控")
                    font.pointSize: 19
                    font.bold: true
                    color: Kirigami.Theme.textColor
                }

                QQC2.Label {
                    text: qsTr("每秒采样 · 最近 60 秒趋势")
                    font.pointSize: 9
                    color: Kirigami.Theme.textColor
                    opacity: 0.5
                }
            }

            Item {
                Layout.fillWidth: true
            }

            QQC2.Button {
                implicitWidth: 34
                implicitHeight: 30
                flat: true
                icon.source: "window-new"
                icon.width: 18
                icon.height: 18
                QQC2.ToolTip.text: qsTr("浮窗监控（总在最前）")
                QQC2.ToolTip.visible: hovered
                QQC2.ToolTip.delay: 500
                onClicked: appWindow.enterFloatMode(page.currentTab)
            }

            QQC2.Button {
                implicitWidth: 34
                implicitHeight: 30
                flat: true
                icon.source: "view-refresh"
                icon.width: 18
                icon.height: 18
                QQC2.ToolTip.text: qsTr("立即刷新")
                QQC2.ToolTip.visible: hovered
                QQC2.ToolTip.delay: 500
                onClicked: {
                    monitor.refresh()
                    appWindow.notify(qsTr("已刷新"))
                }
            }

            QQC2.Button {
                implicitWidth: 34
                implicitHeight: 30
                flat: true
                icon.source: "edit-clear"
                icon.width: 18
                icon.height: 18
                QQC2.ToolTip.text: qsTr("清空历史数据")
                QQC2.ToolTip.visible: hovered
                QQC2.ToolTip.delay: 500
                onClicked: {
                    monitor.clearHistory()
                    appWindow.notify(qsTr("历史数据已清空"))
                }
            }

            QQC2.Button {
                implicitWidth: 34
                implicitHeight: 30
                flat: true
                icon.source: "document-export"
                icon.width: 18
                icon.height: 18
                QQC2.ToolTip.text: qsTr("导出报告")
                QQC2.ToolTip.visible: hovered
                QQC2.ToolTip.delay: 500
                onClicked: {
                    const path = monitor.exportReport()
                    if (path.length > 0) {
                        appWindow.notify(qsTr("报告已保存至 %1").arg(path))
                    } else {
                        appWindow.notify(qsTr("报告导出失败"))
                    }
                }
            }

            QQC2.Label {
                text: qsTr("系统运行 %1").arg(monitor.formatUptime(monitor.uptimeSeconds))
                font.pointSize: 8.5
                color: Kirigami.Theme.textColor
                opacity: 0.45
            }
        }

        TabBar {
            labels: (Translator.language, ["CPU", qsTr("内存"), qsTr("网络"), qsTr("磁盘"), qsTr("温度")])
            currentIndex: page.currentTab
            onActivated: function(index) {
                page.currentTab = index
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: page.currentTab

            // ---------------------------------------------------------------- CPU
            ColumnLayout {
                spacing: 14

                Panel {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 220

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8

                        RowLayout {
                            QQC2.Label {
                                text: qsTr("CPU 总占用率")
                                font.pointSize: 10
                                font.bold: true
                                color: Kirigami.Theme.textColor
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            QQC2.Label {
                                text: monitor.formatPercent(monitor.cpuPercent, 1)
                                font.pointSize: 22
                                font.bold: true
                                color: Kirigami.Theme.positiveTextColor
                            }
                        }

                        LineChart {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            values: monitor.cpuHistory
                            maxValue: 100
                            color1: Kirigami.Theme.positiveTextColor
                        }

                        QQC2.Label {
                            text: (Translator.language, appSettings.tempUnit,
                                    qsTr("温度 %1 · %2 个逻辑核心")
                                        .arg(page.tempText(monitor.cpuTemp))
                                        .arg(systemInfo.cpuCores))
                            font.pointSize: 8
                            color: Kirigami.Theme.textColor
                            opacity: 0.45
                        }
                    }
                }

                Panel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 12

                        QQC2.Label {
                            text: qsTr("每核心占用")
                            font.pointSize: 10
                            font.bold: true
                            color: Kirigami.Theme.textColor
                        }

                        GridLayout {
                            Layout.fillWidth: true
                            columns: 4
                            columnSpacing: 22
                            rowSpacing: 12

                            Repeater {
                                model: monitor.perCore.length

                                delegate: ColumnLayout {
                                    id: coreItem

                                    required property int index

                                    Layout.fillWidth: true
                                    spacing: 3

                                    RowLayout {
                                        spacing: 4

                                        QQC2.Label {
                                            text: qsTr("核心 %1").arg(coreItem.index)
                                            font.pointSize: 8
                                            color: Kirigami.Theme.textColor
                                            opacity: 0.6
                                        }

                                        Item {
                                            Layout.fillWidth: true
                                        }

                                        QQC2.Label {
                                            text: Math.round(monitor.perCore[coreItem.index] || 0) + "%"
                                            font.pointSize: 8
                                            color: Kirigami.Theme.textColor
                                            opacity: 0.8
                                        }
                                    }

                                    UsageBar {
                                        Layout.fillWidth: true
                                        value: monitor.perCore[coreItem.index] || 0
                                        fillColor: Kirigami.Theme.positiveTextColor
                                    }
                                }
                            }
                        }

                        Item {
                            Layout.fillHeight: true
                        }

                        QQC2.Label {
                            text: systemInfo.cpuModel
                            font.pointSize: 8
                            color: Kirigami.Theme.textColor
                            opacity: 0.35
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                    }
                }
            }

            // --------------------------------------------------------------- 内存
            ColumnLayout {
                spacing: 14

                Panel {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 220

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8

                        RowLayout {
                            QQC2.Label {
                                text: qsTr("内存占用")
                                font.pointSize: 10
                                font.bold: true
                                color: Kirigami.Theme.textColor
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            QQC2.Label {
                                text: monitor.formatPercent(monitor.memPercent, 1)
                                font.pointSize: 22
                                font.bold: true
                                color: Kirigami.Theme.highlightColor
                            }
                        }

                        QQC2.Label {
                            text: monitor.formatBytes(monitor.memUsed) + " / " + monitor.formatBytes(monitor.memTotal)
                            font.pointSize: 9
                            color: Kirigami.Theme.textColor
                            opacity: 0.55
                        }

                        UsageBar {
                            Layout.fillWidth: true
                            value: monitor.memPercent
                            trackHeight: 8
                        }

                        LineChart {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            values: monitor.memHistory
                            maxValue: 100
                            color1: Kirigami.Theme.highlightColor
                        }
                    }
                }

                Panel {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8

                        RowLayout {
                            QQC2.Label {
                                text: qsTr("交换空间")
                                font.pointSize: 10
                                font.bold: true
                                color: Kirigami.Theme.textColor
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            QQC2.Label {
                                text: monitor.swapTotal > 0
                                      ? monitor.formatBytes(monitor.swapUsed) + " / " + monitor.formatBytes(monitor.swapTotal)
                                      : qsTr("未启用")
                                font.pointSize: 9
                                color: Kirigami.Theme.textColor
                                opacity: 0.55
                            }
                        }

                        UsageBar {
                            Layout.fillWidth: true
                            value: monitor.swapTotal > 0 ? monitor.swapUsed * 100 / monitor.swapTotal : 0
                            trackHeight: 8
                            fillColor: Kirigami.Theme.neutralTextColor
                        }

                        Item {
                            Layout.fillHeight: true
                        }

                        QQC2.Label {
                            text: (Translator.language, appSettings.tempUnit, qsTr("内存温度 %1").arg(page.tempText(monitor.memTemp)))
                            font.pointSize: 8
                            color: Kirigami.Theme.textColor
                            opacity: 0.4
                        }
                    }
                }
            }

            // --------------------------------------------------------------- 网络
            ColumnLayout {
                spacing: 14

                Panel {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 220

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8

                        RowLayout {
                            spacing: 24

                            QQC2.Label {
                                text: qsTr("网络吞吐")
                                font.pointSize: 10
                                font.bold: true
                                color: Kirigami.Theme.textColor
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            ColumnLayout {
                                spacing: 0

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: qsTr("↑ %1").arg(monitor.formatRate(monitor.netUp))
                                    font.pointSize: 13
                                    font.bold: true
                                    color: Kirigami.Theme.positiveTextColor
                                }

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: qsTr("↓ %1").arg(monitor.formatRate(monitor.netDown))
                                    font.pointSize: 13
                                    font.bold: true
                                    color: Kirigami.Theme.highlightColor
                                }
                            }
                        }

                        LineChart {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            values: monitor.netDownHistory
                            values2: monitor.netUpHistory
                            color1: Kirigami.Theme.highlightColor
                            color2: Kirigami.Theme.positiveTextColor
                        }
                    }
                }

                Panel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8

                        QQC2.Label {
                            text: qsTr("网络接口")
                            font.pointSize: 10
                            font.bold: true
                            color: Kirigami.Theme.textColor
                        }

                        QQC2.Label {
                            visible: monitor.interfaces.length === 0
                            text: qsTr("当前没有活动网络接口")
                            font.pointSize: 8.5
                            color: Kirigami.Theme.textColor
                            opacity: 0.4
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            Repeater {
                                model: monitor.interfaces

                                delegate: RowLayout {
                                    required property var modelData

                                    Layout.fillWidth: true
                                    spacing: 12

                                    Kirigami.Icon {
                                        source: "network-wired"
                                        Layout.preferredWidth: 18
                                        Layout.preferredHeight: 18
                                        opacity: 0.7
                                    }

                                    QQC2.Label {
                                        text: modelData.name
                                        Layout.fillWidth: true
                                        font.pointSize: 9
                                        color: Kirigami.Theme.textColor
                                    }

                                    QQC2.Label {
                                        text: qsTr("↑ %1").arg(monitor.formatRate(modelData.up))
                                        font.pointSize: 8.5
                                        color: Kirigami.Theme.positiveTextColor
                                    }

                                    QQC2.Label {
                                        text: qsTr("↓ %1").arg(monitor.formatRate(modelData.down))
                                        font.pointSize: 8.5
                                        color: Kirigami.Theme.highlightColor
                                    }

                                    Item {
                                        Layout.preferredWidth: 20
                                    }
                                }
                            }
                        }

                        Item {
                            Layout.fillHeight: true
                        }
                    }
                }
            }

            // --------------------------------------------------------------- 磁盘
            ColumnLayout {
                spacing: 14

                Panel {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 200

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8

                        RowLayout {
                            QQC2.Label {
                                text: qsTr("磁盘吞吐")
                                font.pointSize: 10
                                font.bold: true
                                color: Kirigami.Theme.textColor
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            ColumnLayout {
                                spacing: 0

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: qsTr("读 %1").arg(monitor.formatRate(monitor.diskRead))
                                    font.pointSize: 12
                                    font.bold: true
                                    color: Kirigami.Theme.highlightColor
                                }

                                QQC2.Label {
                                    Layout.alignment: Qt.AlignRight
                                    text: qsTr("写 %1").arg(monitor.formatRate(monitor.diskWrite))
                                    font.pointSize: 12
                                    font.bold: true
                                    color: Kirigami.Theme.neutralTextColor
                                }
                            }
                        }

                        LineChart {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            values: monitor.diskReadHistory
                            values2: monitor.diskWriteHistory
                            color1: Kirigami.Theme.highlightColor
                            color2: Kirigami.Theme.neutralTextColor
                        }
                    }
                }

                Panel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 10

                        QQC2.Label {
                            text: qsTr("磁盘使用率")
                            font.pointSize: 10
                            font.bold: true
                            color: Kirigami.Theme.textColor
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 12

                            Repeater {
                                model: monitor.mounts

                                delegate: ColumnLayout {
                                    required property var modelData

                                    Layout.fillWidth: true
                                    spacing: 4

                                    RowLayout {
                                        spacing: 8

                                        QQC2.Label {
                                            text: modelData.name
                                            font.pointSize: 9
                                            color: Kirigami.Theme.textColor
                                        }

                                        QQC2.Label {
                                            text: modelData.mount
                                            font.pointSize: 8
                                            color: Kirigami.Theme.textColor
                                            opacity: 0.4
                                            Layout.fillWidth: true
                                            elide: Text.ElideRight
                                        }

                                        QQC2.Label {
                                            text: monitor.formatBytes(modelData.used) + " / " + monitor.formatBytes(modelData.total)
                                            font.pointSize: 8.5
                                            color: Kirigami.Theme.textColor
                                            opacity: 0.6
                                        }

                                        QQC2.Label {
                                            text: monitor.formatPercent(modelData.percent, 0)
                                            font.pointSize: 8.5
                                            font.bold: true
                                            color: modelData.percent > 85 ? Kirigami.Theme.negativeTextColor
                                                                         : Kirigami.Theme.textColor
                                        }
                                    }

                                    UsageBar {
                                        Layout.fillWidth: true
                                        value: modelData.percent
                                        fillColor: modelData.percent > 85 ? Kirigami.Theme.negativeTextColor
                                                                          : Kirigami.Theme.highlightColor
                                    }
                                }
                            }
                        }

                        Item {
                            Layout.fillHeight: true
                        }
                    }
                }
            }

            // --------------------------------------------------------------- 温度
            ColumnLayout {
                spacing: 14

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 14

                    Panel {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 120

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 6

                            RowLayout {
                                Kirigami.Icon {
                                    source: "cpu"
                                    Layout.preferredWidth: 20
                                    Layout.preferredHeight: 20
                                    color: Kirigami.Theme.positiveTextColor
                                }

                                QQC2.Label {
                                    text: qsTr("CPU 温度")
                                    font.pointSize: 9
                                    color: Kirigami.Theme.textColor
                                    opacity: 0.6
                                }
                            }

                            QQC2.Label {
                                text: (Translator.language, appSettings.tempUnit, page.tempText(monitor.cpuTemp))
                                font.pointSize: 20
                                font.bold: true
                                color: monitor.cpuTemp > 80 ? Kirigami.Theme.negativeTextColor
                                                            : Kirigami.Theme.textColor
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }
                    }

                    Panel {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 120

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 6

                            RowLayout {
                                Kirigami.Icon {
                                    source: "memory"
                                    Layout.preferredWidth: 20
                                    Layout.preferredHeight: 20
                                    color: Kirigami.Theme.highlightColor
                                }

                                QQC2.Label {
                                    text: qsTr("内存温度")
                                    font.pointSize: 9
                                    color: Kirigami.Theme.textColor
                                    opacity: 0.6
                                }
                            }

                            QQC2.Label {
                                text: (Translator.language, appSettings.tempUnit, page.tempText(monitor.memTemp))
                                font.pointSize: 20
                                font.bold: true
                                color: Kirigami.Theme.textColor
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }
                    }
                }

                Panel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8

                        QQC2.Label {
                            text: qsTr("温度传感器")
                            font.pointSize: 10
                            font.bold: true
                            color: Kirigami.Theme.textColor
                        }

                        QQC2.Label {
                            visible: monitor.sensors.length === 0
                            text: qsTr("当前系统未向 /sys/class/hwmon 暴露温度传感器。")
                            font.pointSize: 8.5
                            color: Kirigami.Theme.textColor
                            opacity: 0.45
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 7

                            Repeater {
                                model: monitor.sensors

                                delegate: RowLayout {
                                    required property var modelData

                                    Layout.fillWidth: true
                                    spacing: 10

                                    QQC2.Label {
                                        text: modelData.label
                                        font.pointSize: 8.5
                                        color: Kirigami.Theme.textColor
                                        Layout.fillWidth: true
                                        elide: Text.ElideRight
                                    }

                                    QQC2.Label {
                                        text: (appSettings.tempUnit, appSettings.toDisplayTemp(modelData.temp).toFixed(1) + " " + appSettings.tempUnitSymbol())
                                        font.pointSize: 8.5
                                        font.bold: true
                                        color: modelData.temp > 80 ? Kirigami.Theme.negativeTextColor
                                                                   : Kirigami.Theme.textColor
                                    }
                                }
                            }
                        }

                        Item {
                            Layout.fillHeight: true
                        }
                    }
                }
            }
        }
    }
}
