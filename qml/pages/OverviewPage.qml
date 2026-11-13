import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import "../components"

Item {
    id: page
    property var appWindow

    function fallback(value) {
        return value.length > 0 ? value : qsTr("未知")
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 16

        ColumnLayout {
            spacing: 2

            QQC2.Label {
                text: qsTr("硬件配置总览")
                font.pointSize: 19
                font.bold: true
                color: Kirigami.Theme.textColor
            }

            QQC2.Label {
                text: systemInfo.hostname + " · " + page.fallback(systemInfo.distro)
                font.pointSize: 9
                color: Kirigami.Theme.textColor
                opacity: 0.5
            }
        }

        GridLayout {
            Layout.fillWidth: true
            columns: 2
            columnSpacing: 16
            rowSpacing: 16

            InfoCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 104
                icon: "computer"
                label: qsTr("发行版")
                value: page.fallback(systemInfo.distro)
                detail: systemInfo.hostname
            }

            InfoCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 104
                icon: "preferences-system"
                label: qsTr("操作系统内核")
                value: page.fallback(systemInfo.kernel)
                detail: systemInfo.arch
                tint: Kirigami.Theme.positiveTextColor
            }

            InfoCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 104
                icon: "cpu"
                label: qsTr("系统位数")
                value: systemInfo.bits + " " + qsTr("位")
                detail: systemInfo.arch
            }

            InfoCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 104
                icon: "memory"
                label: qsTr("内存大小")
                value: page.fallback(systemInfo.memory)
                detail: qsTr("物理内存总量")
                tint: Kirigami.Theme.neutralTextColor
            }

            InfoCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 104
                icon: "video-display"
                label: qsTr("显卡型号")
                value: page.fallback(systemInfo.gpu)
                detail: qsTr("图形处理器")
                tint: Kirigami.Theme.positiveTextColor
            }

            InfoCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 104
                icon: "preferences-desktop-display"
                label: qsTr("会话类型")
                value: page.fallback(systemInfo.sessionType)
                detail: qsTr("图形会话协议")
            }

            InfoCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 104
                icon: "preferences-desktop"
                label: qsTr("桌面类型")
                value: page.fallback(systemInfo.desktop)
                detail: qsTr("当前桌面环境")
                tint: Kirigami.Theme.neutralTextColor
            }

            InfoCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 104
                icon: "cpu"
                label: qsTr("处理器")
                value: page.fallback(systemInfo.cpuModel)
                detail: qsTr("%1 个逻辑核心").arg(systemInfo.cpuCores)
                tint: Kirigami.Theme.positiveTextColor
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
