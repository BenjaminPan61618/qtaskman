import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import qtaskman 1.0
import "../components"

Item {
    id: page
    property var appWindow

    function langIndex() {
        const code = Translator.language
        const langs = Translator.languages
        for (let i = 0; i < langs.length; ++i) {
            if (langs[i].code === code) return i
        }
        return 0
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 20

        ColumnLayout {
            spacing: 2

            QQC2.Label {
                text: qsTr("设置")
                font.pointSize: 19
                font.bold: true
                color: Kirigami.Theme.textColor
            }

            QQC2.Label {
                text: qsTr("界面语言、外观主题、温度单位、数据采集频率、后台驻留与浮窗透明度")
                font.pointSize: 9
                color: Kirigami.Theme.textColor
                opacity: 0.5
            }
        }

        // ---------------------------------------------------- 外观主题
        Panel {
            Layout.fillWidth: true
            Layout.preferredHeight: 100

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12

                RowLayout {
                    Layout.fillWidth: true

                    Kirigami.Icon {
                        source: "preferences-desktop-theme"
                        Layout.preferredWidth: 22
                        Layout.preferredHeight: 22
                    }

                    QQC2.Label {
                        text: qsTr("外观主题")
                        font.pointSize: 11
                        font.bold: true
                        color: Kirigami.Theme.textColor
                    }

                    Item { Layout.fillWidth: true }

                    QQC2.ComboBox {
                        id: themeCombo
                        implicitWidth: 200
                        model: [
                            { text: qsTr("跟随系统"), value: "system" },
                            { text: qsTr("浅色"), value: "light" },
                            { text: qsTr("深色"), value: "dark" }
                        ]
                        textRole: "text"
                        currentIndex: {
                            const m = appSettings.themeMode
                            if (m === "light") return 1
                            if (m === "dark") return 2
                            return 0
                        }
                        onActivated: appSettings.setThemeMode(model[currentIndex].value)
                    }
                }

                QQC2.Label {
                    text: qsTr("选择应用配色方案，跟随系统时会随系统的明暗主题自动切换。")
                    font.pointSize: 8.5
                    color: Kirigami.Theme.textColor
                    opacity: 0.5
                }
            }
        }

        // ---------------------------------------------------- 语言
        Panel {
            Layout.fillWidth: true
            Layout.preferredHeight: 100

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12

                RowLayout {
                    Layout.fillWidth: true

                    Kirigami.Icon {
                        source: "preferences-desktop-locale"
                        Layout.preferredWidth: 22
                        Layout.preferredHeight: 22
                    }

                    QQC2.Label {
                        text: qsTr("界面语言")
                        font.pointSize: 11
                        font.bold: true
                        color: Kirigami.Theme.textColor
                    }

                    Item { Layout.fillWidth: true }

                    QQC2.ComboBox {
                        id: langCombo
                        implicitWidth: 200
                        model: Translator.languages
                        textRole: "name"
                        currentIndex: page.langIndex()
                        onActivated: Translator.setLanguage(model[currentIndex].code)
                    }
                }

                QQC2.Label {
                    text: qsTr("选择应用界面显示语言，切换后立即生效。")
                    font.pointSize: 8.5
                    color: Kirigami.Theme.textColor
                    opacity: 0.5
                }
            }
        }

        // ---------------------------------------------------- 温度单位
        Panel {
            Layout.fillWidth: true
            Layout.preferredHeight: 100

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12

                RowLayout {
                    Layout.fillWidth: true

                    Kirigami.Icon {
                        source: "temperature"
                        Layout.preferredWidth: 22
                        Layout.preferredHeight: 22
                    }

                    QQC2.Label {
                        text: qsTr("温度单位")
                        font.pointSize: 11
                        font.bold: true
                        color: Kirigami.Theme.textColor
                    }

                    Item { Layout.fillWidth: true }

                    QQC2.ComboBox {
                        id: tempCombo
                        implicitWidth: 200
                        model: [
                            { text: qsTr("摄氏度 (°C)"), value: "C" },
                            { text: qsTr("华氏度 (°F)"), value: "F" }
                        ]
                        textRole: "text"
                        currentIndex: appSettings.tempUnit === "F" ? 1 : 0
                        onActivated: appSettings.setTempUnit(model[currentIndex].value)
                    }
                }

                QQC2.Label {
                    text: qsTr("硬件监控页面中温度数值的显示单位。")
                    font.pointSize: 8.5
                    color: Kirigami.Theme.textColor
                    opacity: 0.5
                }
            }
        }

        // ---------------------------------------------------- 采集间隔
        Panel {
            Layout.fillWidth: true
            Layout.preferredHeight: 100

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12

                RowLayout {
                    Layout.fillWidth: true

                    Kirigami.Icon {
                        source: "chronometer"
                        Layout.preferredWidth: 22
                        Layout.preferredHeight: 22
                    }

                    QQC2.Label {
                        text: qsTr("数据采集间隔")
                        font.pointSize: 11
                        font.bold: true
                        color: Kirigami.Theme.textColor
                    }

                    Item { Layout.fillWidth: true }

                    QQC2.ComboBox {
                        id: intervalCombo
                        implicitWidth: 200
                        model: [
                            { text: qsTr("0.5 秒"), value: 500 },
                            { text: qsTr("1 秒"), value: 1000 },
                            { text: qsTr("2 秒"), value: 2000 },
                            { text: qsTr("5 秒"), value: 5000 }
                        ]
                        textRole: "text"
                        currentIndex: {
                            const v = appSettings.sampleInterval
                            if (v <= 500) return 0
                            if (v <= 1000) return 1
                            if (v <= 2000) return 2
                            return 3
                        }
                        onActivated: appSettings.setSampleInterval(model[currentIndex].value)
                    }
                }

                QQC2.Label {
                    text: qsTr("系统资源数据的采样频率，间隔越短实时性越高，但 CPU 占用也会增加。")
                    font.pointSize: 8.5
                    color: Kirigami.Theme.textColor
                    opacity: 0.5
                }
            }
        }

        // ---------------------------------------------------- 后台驻留
        Panel {
            Layout.fillWidth: true
            Layout.preferredHeight: 100

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12

                RowLayout {
                    Layout.fillWidth: true

                    Kirigami.Icon {
                        source: "view-restore"
                        Layout.preferredWidth: 22
                        Layout.preferredHeight: 22
                    }

                    QQC2.Label {
                        text: qsTr("关闭窗口时驻留到系统托盘")
                        font.pointSize: 11
                        font.bold: true
                        color: Kirigami.Theme.textColor
                    }

                    Item { Layout.fillWidth: true }

                    QQC2.Switch {
                        id: traySwitch
                        checked: appSettings.stayInTray
                        enabled: trayAvailable
                        onToggled: appSettings.setStayInTray(checked)
                    }
                }

                QQC2.Label {
                    text: qsTr("开启后关闭主窗口只会隐藏到系统托盘，程序继续在后台运行；关闭时退出程序并移除托盘图标。")
                    font.pointSize: 8.5
                    color: Kirigami.Theme.textColor
                    opacity: 0.5
                }
            }
        }

        // ---------------------------------------------------- 浮窗透明度
        Panel {
            Layout.fillWidth: true
            Layout.preferredHeight: 100

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12

                RowLayout {
                    Layout.fillWidth: true

                    Kirigami.Icon {
                        source: "window-duplicate"
                        Layout.preferredWidth: 22
                        Layout.preferredHeight: 22
                    }

                    QQC2.Label {
                        text: qsTr("浮窗透明度")
                        font.pointSize: 11
                        font.bold: true
                        color: Kirigami.Theme.textColor
                    }

                    Item { Layout.fillWidth: true }

                    QQC2.Label {
                        text: qsTr("%1%").arg(Math.round(appSettings.floatOpacity * 100))
                        font.pointSize: 10
                        color: Kirigami.Theme.textColor
                        opacity: 0.7
                        Layout.minimumWidth: 56
                        horizontalAlignment: Text.AlignRight
                    }
                }

                QQC2.Slider {
                    Layout.fillWidth: true
                    from: 0.2
                    to: 1.0
                    stepSize: 0.05
                    value: appSettings.floatOpacity
                    onMoved: appSettings.setFloatOpacity(value)
                }

                QQC2.Label {
                    text: qsTr("性能监控浮窗的整体不透明度，最低为 20%，不会完全透明。")
                    font.pointSize: 8.5
                    color: Kirigami.Theme.textColor
                    opacity: 0.5
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
