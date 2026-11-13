import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import QtQuick.Window as QW
import org.kde.kirigami as Kirigami
import qtaskman 1.0

QW.Window {
    id: win

    // Root window, used to restore it when the floating window is dismissed.
    property var appWindow
    // 0 CPU, 1 内存, 2 网络, 3 磁盘, 4 温度
    property int category: 0
    property var categories: (Translator.language,
                              ["CPU", qsTr("内存"), qsTr("网络"), qsTr("磁盘"), qsTr("温度")])

    width: 380
    height: 250
    minimumWidth: 300
    minimumHeight: 200
    // 窗口本身透明，请求带 alpha 通道的可视类型。
    // 透明度不放在 Window.opacity 上（Wayland 不支持窗口级 opacity），
    // 而是应用到下方 content 层，靠内容的 alpha 通道实现透明。
    color: "transparent"
    // 普通顶层窗口 + 无边框 + 总在最前。
    // 不使用 Qt.Tool：工具窗口在部分窗口管理器（含 GNOME/Mutter）上不响应 stays-on-top。
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    // 浮窗是独立的顶层窗口：断开与主窗口的瞬时父子关系，
    // 否则主窗口 hide() 时平台会把瞬时子窗口一起隐藏。
    transientParent: null
    title: qsTr("性能浮窗")

    // 在原生窗口创建完成后再确保一次瞬时父窗口为空，
    // 防止 QML 声明式赋值被父窗口的自动关联覆盖。
    Component.onCompleted: transientParent = null

    onClosing: win.appWindow.leaveFloatMode()

    QtObject {
        id: theme

        readonly property color faint: Qt.rgba(Kirigami.Theme.textColor.r,
                                               Kirigami.Theme.textColor.g,
                                               Kirigami.Theme.textColor.b, 0.12)
        readonly property color pill: Qt.rgba(Kirigami.Theme.textColor.r,
                                              Kirigami.Theme.textColor.g,
                                              Kirigami.Theme.textColor.b, 0.07)
        readonly property color accent: Qt.rgba(Kirigami.Theme.highlightColor.r,
                                                Kirigami.Theme.highlightColor.g,
                                                Kirigami.Theme.highlightColor.b, 0.85)
    }

    function tempText(value) {
        if (value < 0) return qsTr("不可用")
        return appSettings.toDisplayTemp(value).toFixed(1) + " " + appSettings.tempUnitSymbol()
    }

    function valueText() {
        switch (category) {
        case 0: return monitor.formatPercent(monitor.cpuPercent, 1)
        case 1: return monitor.formatPercent(monitor.memPercent, 1)
        case 2: return qsTr("↓ %1").arg(monitor.formatRate(monitor.netDown))
        case 3: return qsTr("读 %1").arg(monitor.formatRate(monitor.diskRead))
        default: return tempText(monitor.cpuTemp)
        }
    }

    function subText() {
        switch (category) {
        case 0: return qsTr("%1 个逻辑核心").arg(systemInfo.cpuCores)
        case 1: return monitor.formatBytes(monitor.memUsed) + " / " + monitor.formatBytes(monitor.memTotal)
        case 2: return qsTr("↑ %1").arg(monitor.formatRate(monitor.netUp))
        case 3: return qsTr("写 %1").arg(monitor.formatRate(monitor.diskWrite))
        default: return qsTr("内存 %1").arg(tempText(monitor.memTemp))
        }
    }

    function valueColor() {
        switch (category) {
        case 0: return Kirigami.Theme.positiveTextColor
        case 1: return Kirigami.Theme.highlightColor
        case 2: return Kirigami.Theme.highlightColor
        case 3: return Kirigami.Theme.highlightColor
        default: return monitor.cpuTemp > 80 ? Kirigami.Theme.negativeTextColor
                                             : Kirigami.Theme.textColor
        }
    }

    function chartValues() {
        switch (category) {
        case 0: return monitor.cpuHistory
        case 1: return monitor.memHistory
        case 2: return monitor.netDownHistory
        case 3: return monitor.diskReadHistory
        default: return monitor.cpuTempHistory
        }
    }

    function chartValues2() {
        switch (category) {
        case 2: return monitor.netUpHistory
        case 3: return monitor.diskWriteHistory
        case 4: return monitor.memTempHistory
        default: return []
        }
    }

    function chartMax() {
        return (category === 0 || category === 1) ? 100 : 0
    }

    function chartColor1() {
        switch (category) {
        case 0: return Kirigami.Theme.positiveTextColor
        case 4: return Kirigami.Theme.negativeTextColor
        default: return Kirigami.Theme.highlightColor
        }
    }

    function chartColor2() {
        switch (category) {
        case 2: return Kirigami.Theme.positiveTextColor
        case 3: return Kirigami.Theme.neutralTextColor
        default: return Kirigami.Theme.highlightColor
        }
    }

    Shortcut {
        sequence: "Esc"
        onActivated: win.appWindow.leaveFloatMode()
    }

    // 内容层：整体透明度应用于此（不靠 Window.opacity，Wayland 不支持窗口级透明度）。
    // 窗口 color 为 transparent，content 的 alpha 通道由合成器混合桌面实现透明。
    Item {
        id: content
        anchors.fill: parent
        opacity: appSettings.floatOpacity

        // 背景 + 边框
        Rectangle {
            anchors.fill: parent
            color: Kirigami.Theme.backgroundColor
            border.width: 1
            border.color: theme.faint
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 8

            // ------------------------------------------------------------ 标题栏
            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 24

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.SizeAllCursor
                    onPressed: win.startSystemMove()
                }

                RowLayout {
                    anchors.fill: parent
                    spacing: 6

                    Kirigami.Icon {
                        source: "utilities-system-monitor"
                        Layout.preferredWidth: 16
                        Layout.preferredHeight: 16
                    }

                    QQC2.Label {
                        text: qsTr("性能监控")
                        font.pointSize: 9
                        font.bold: true
                        color: Kirigami.Theme.textColor
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    QQC2.Button {
                        implicitWidth: 24
                        implicitHeight: 22
                        flat: true
                        icon.source: "window-close"
                        icon.width: 14
                        icon.height: 14
                        QQC2.ToolTip.text: qsTr("返回主窗口")
                        QQC2.ToolTip.visible: hovered
                        QQC2.ToolTip.delay: 500
                        onClicked: win.appWindow.leaveFloatMode()
                    }
                }
            }

            // ------------------------------------------------------------ 类目
            RowLayout {
                Layout.fillWidth: true
                spacing: 4

                Repeater {
                    model: win.categories

                    delegate: Rectangle {
                        id: pill

                        required property string modelData
                        required property int index

                        Layout.preferredWidth: pillLabel.implicitWidth + 14
                        Layout.preferredHeight: 20
                        radius: 10
                        color: pill.index === win.category ? theme.accent : theme.pill

                        QQC2.Label {
                            id: pillLabel
                            anchors.centerIn: parent
                            text: pill.modelData
                            font.pointSize: 7.5
                            color: pill.index === win.category ? Kirigami.Theme.highlightedTextColor
                                                               : Kirigami.Theme.textColor
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: win.category = pill.index
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                }
            }

            // ------------------------------------------------------------ 数值
            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                QQC2.Label {
                    text: win.valueText()
                    font.pointSize: 19
                    font.bold: true
                    color: win.valueColor()
                    elide: Text.ElideRight
                }

                Item {
                    Layout.fillWidth: true
                }

                QQC2.Label {
                    text: win.subText()
                    font.pointSize: 8
                    color: Kirigami.Theme.textColor
                    opacity: 0.55
                    horizontalAlignment: Text.AlignRight
                    elide: Text.ElideLeft
                }
            }

            LineChart {
                Layout.fillWidth: true
                Layout.fillHeight: true
                values: win.chartValues()
                values2: win.chartValues2()
                maxValue: win.chartMax()
                color1: win.chartColor1()
                color2: win.chartColor2()
            }
        }

        MouseArea {
            width: 16
            height: 16
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            cursorShape: Qt.SizeFDiagCursor
            onPressed: win.startSystemResize(Qt.RightEdge | Qt.BottomEdge)
        }
    }
}
