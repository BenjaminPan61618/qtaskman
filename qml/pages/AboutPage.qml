import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import qtaskman 1.0
import "../components"

Item {
    id: page
    property var appWindow

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 20

        ColumnLayout {
            spacing: 2

            QQC2.Label {
                text: qsTr("关于")
                font.pointSize: 19
                font.bold: true
                color: Kirigami.Theme.textColor
            }

            QQC2.Label {
                text: qsTr("应用信息与运行环境")
                font.pointSize: 9
                color: Kirigami.Theme.textColor
                opacity: 0.5
            }
        }

        // ---------------------------------------------------- 应用信息
        Panel {
            Layout.fillWidth: true
            Layout.preferredHeight: 160

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 18

                Kirigami.Icon {
                    source: "utilities-system-monitor"
                    Layout.preferredWidth: 72
                    Layout.preferredHeight: 72
                }

                ColumnLayout {
                    spacing: 6

                    QQC2.Label {
                        text: "qTaskMan"
                        font.pointSize: 18
                        font.bold: true
                        color: Kirigami.Theme.textColor
                    }

                    QQC2.Label {
                        text: qsTr("版本 %1").arg(Qt.application.version)
                        font.pointSize: 10
                        color: Kirigami.Theme.textColor
                        opacity: 0.7
                    }

                    QQC2.Label {
                        text: qsTr("基于 Qt 6 与 Kirigami 的系统任务管理器")
                        font.pointSize: 9
                        color: Kirigami.Theme.textColor
                        opacity: 0.6
                    }

                    Item { Layout.fillHeight: true }

                    QQC2.Label {
                        text: qsTr("© 2026 qTaskMan 开发者")
                        font.pointSize: 8.5
                        color: Kirigami.Theme.textColor
                        opacity: 0.45
                    }
                }

                Item { Layout.fillWidth: true }
            }
        }

        // ---------------------------------------------------- 系统信息
        Panel {
            Layout.fillWidth: true
            Layout.preferredHeight: 200

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 18
                spacing: 10

                QQC2.Label {
                    text: qsTr("系统信息")
                    font.pointSize: 11
                    font.bold: true
                    color: Kirigami.Theme.textColor
                }

                GridLayout {
                    Layout.fillWidth: true
                    columns: 2
                    columnSpacing: 24
                    rowSpacing: 8

                    QQC2.Label {
                        text: qsTr("操作系统")
                        font.pointSize: 9
                        color: Kirigami.Theme.textColor
                        opacity: 0.55
                    }
                    QQC2.Label {
                        text: systemInfo.distro
                        font.pointSize: 9
                        color: Kirigami.Theme.textColor
                    }

                    QQC2.Label {
                        text: qsTr("内核")
                        font.pointSize: 9
                        color: Kirigami.Theme.textColor
                        opacity: 0.55
                    }
                    QQC2.Label {
                        text: systemInfo.kernel
                        font.pointSize: 9
                        color: Kirigami.Theme.textColor
                    }

                    QQC2.Label {
                        text: qsTr("CPU")
                        font.pointSize: 9
                        color: Kirigami.Theme.textColor
                        opacity: 0.55
                    }
                    QQC2.Label {
                        text: systemInfo.cpuModel
                        font.pointSize: 9
                        color: Kirigami.Theme.textColor
                        elide: Text.ElideRight
                    }

                    QQC2.Label {
                        text: qsTr("内存")
                        font.pointSize: 9
                        color: Kirigami.Theme.textColor
                        opacity: 0.55
                    }
                    QQC2.Label {
                        text: systemInfo.memory
                        font.pointSize: 9
                        color: Kirigami.Theme.textColor
                    }

                    QQC2.Label {
                        text: qsTr("桌面环境")
                        font.pointSize: 9
                        color: Kirigami.Theme.textColor
                        opacity: 0.55
                    }
                    QQC2.Label {
                        text: systemInfo.desktop
                        font.pointSize: 9
                        color: Kirigami.Theme.textColor
                    }

                    QQC2.Label {
                        text: qsTr("图形界面")
                        font.pointSize: 9
                        color: Kirigami.Theme.textColor
                        opacity: 0.55
                    }
                    QQC2.Label {
                        text: systemInfo.sessionType
                        font.pointSize: 9
                        color: Kirigami.Theme.textColor
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
