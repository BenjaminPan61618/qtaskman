import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import "pages" as Pages
import "components" as Components

QQC2.ApplicationWindow {
    id: window
    visible: true
    width: 1400
    height: 900
    minimumWidth: 1120
    minimumHeight: 720
    title: "qTaskMan"
    color: Kirigami.Theme.backgroundColor

    property int section: 0
    property int monitorTab: 0
    // 浮窗动态创建（无 QObject 父对象），保证它是真正的顶层窗口，
    // 在 Wayland 上才能作为 xdg-toplevel 获得 stays-on-top 状态。
    property var floatWindow: null

    onMonitorTabChanged: monitorPage.setTab(monitorTab)

    onClosing: function(close) {
        if (trayAvailable && appSettings.stayInTray) {
            close.accepted = false
            window.hide()
        }
    }

    onVisibleChanged: {
        // The main window coming back (e.g. from the tray) dismisses the float.
        if (visible && floatWindow) {
            floatWindow.hide()
        }
    }

    function enterFloatMode(index) {
        if (floatWindow === null) {
            floatWindow = floatComponent.createObject(null, { "appWindow": window })
        }
        floatWindow.category = (index === undefined) ? monitorPage.currentTab : index
        if (floatWindow.x === 0 && floatWindow.y === 0) {
            // Put it near the top-right corner of the main window by default.
            floatWindow.x = window.x + window.width - floatWindow.width - 40
            floatWindow.y = window.y + 40
        }
        // 先隐藏主窗口、再显示浮窗。
        window.hide()
        floatWindow.show()
        floatWindow.raise()
        floatWindow.requestActivate()
    }

    function leaveFloatMode() {
        window.show()
        window.raise()
        window.requestActivate()
        if (floatWindow) {
            floatWindow.destroy()
            floatWindow = null
        }
    }

    Sidebar {
        id: sidebar
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 252
        currentIndex: window.section
        processCount: processModel.count
        appCount: appModel.count
        onNavigate: function(index) {
            window.section = index
        }
        onOpenMonitor: function(tab) {
            window.section = 3
            monitorPage.setTab(tab)
        }
        onOpenSettings: function() {
            window.section = 4
        }
        onOpenAbout: function() {
            window.section = 5
        }
    }

    Item {
        anchors.left: sidebar.right
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        StackLayout {
            anchors.fill: parent
            currentIndex: window.section

            Pages.OverviewPage {}
            Pages.AppsPage {
                appWindow: window
            }
            Pages.ProcessesPage {
                id: processesPage
                appWindow: window
            }
            Pages.MonitorPage {
                id: monitorPage
                appWindow: window
            }
            Pages.SettingsPage {
                appWindow: window
            }
            Pages.AboutPage {
                appWindow: window
            }
        }
    }

    function showProcess(pid) {
        window.section = 2
        processesPage.focusPid(pid)
    }

    function notify(message) {
        if (!message) {
            return
        }
        toast.show(message)
    }

    Components.Toast {
        id: toast
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 28
        z: 100
    }

    // 浮窗组件：通过 createObject(null) 动态实例化，不挂在主窗口下，
    // 确保原生窗口是独立顶层（Wayland xdg-toplevel），stays-on-top 才生效。
    Component {
        id: floatComponent
        Components.FloatMonitorWindow {}
    }
}
