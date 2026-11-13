import QtQuick
import QtQuick.Controls as QQC2

QQC2.Menu {
    id: menu

    property int pid: -1
    property string processName: ""
    property var appWindow: null

    function currentName() {
        return processName.length > 0 ? processName : qsTr("PID %1").arg(pid)
    }

    function sendSignal(signalNumber, name) {
        if (pid <= 0) {
            return
        }
        const ok = procController.sendSignal(pid, signalNumber)
        if (appWindow) {
            appWindow.notify(ok ? qsTr("已向 %1 发送 %2").arg(currentName()).arg(name)
                                : procController.lastError)
        }
    }

    function setPriority(niceValue, label) {
        if (pid <= 0) {
            return
        }
        const ok = procController.setPriority(pid, niceValue)
        if (appWindow) {
            appWindow.notify(ok ? qsTr("已将 %1 的优先级设为 %2").arg(currentName()).arg(label)
                                : procController.lastError)
        }
    }

    function openExecutable() {
        if (pid <= 0) {
            return
        }
        const path = procController.executablePath(pid)
        const ok = procController.openExecutableLocation(pid)
        if (appWindow) {
            appWindow.notify(ok ? qsTr("已打开 %1 所在位置").arg(path)
                                : procController.lastError)
        }
    }

    function restart() {
        if (pid <= 0) {
            return
        }
        const ok = procController.restartProcess(pid)
        if (appWindow) {
            appWindow.notify(ok ? qsTr("已重新启动 %1").arg(currentName())
                                : procController.lastError)
        }
    }

    QQC2.MenuItem {
        text: qsTr("在进程管理中查看")
        enabled: menu.pid > 0
        onTriggered: {
            if (menu.appWindow) {
                menu.appWindow.showProcess(menu.pid)
            }
        }
    }

    QQC2.MenuSeparator {}

    QQC2.MenuItem {
        text: qsTr("打开可执行文件位置")
        enabled: menu.pid > 0
        onTriggered: menu.openExecutable()
    }

    QQC2.MenuItem {
        text: qsTr("重新启动进程")
        enabled: menu.pid > 0
        onTriggered: menu.restart()
    }

    QQC2.MenuSeparator {}

    QQC2.MenuItem {
        text: qsTr("退出进程 (SIGTERM)")
        enabled: menu.pid > 0
        onTriggered: menu.sendSignal(15, "SIGTERM")
    }

    QQC2.MenuItem {
        text: qsTr("强制结束 (SIGKILL)")
        enabled: menu.pid > 0
        onTriggered: menu.sendSignal(9, "SIGKILL")
    }

    QQC2.MenuSeparator {}

    QQC2.Menu {
        title: qsTr("发送信号")
        enabled: menu.pid > 0

        QQC2.MenuItem { text: qsTr("SIGTERM · 终止"); onTriggered: menu.sendSignal(15, "SIGTERM") }
        QQC2.MenuItem { text: qsTr("SIGKILL · 强制终止"); onTriggered: menu.sendSignal(9, "SIGKILL") }
        QQC2.MenuItem { text: qsTr("SIGINT · 中断"); onTriggered: menu.sendSignal(2, "SIGINT") }
        QQC2.MenuItem { text: qsTr("SIGHUP · 挂起"); onTriggered: menu.sendSignal(1, "SIGHUP") }
        QQC2.MenuItem { text: qsTr("SIGSTOP · 暂停"); onTriggered: menu.sendSignal(19, "SIGSTOP") }
        QQC2.MenuItem { text: qsTr("SIGCONT · 继续"); onTriggered: menu.sendSignal(18, "SIGCONT") }
        QQC2.MenuItem { text: "SIGUSR1"; onTriggered: menu.sendSignal(10, "SIGUSR1") }
        QQC2.MenuItem { text: "SIGUSR2"; onTriggered: menu.sendSignal(12, "SIGUSR2") }
    }

    QQC2.Menu {
        title: qsTr("设置优先级")
        enabled: menu.pid > 0

        QQC2.MenuItem { text: qsTr("最高 (-20，需要权限)"); onTriggered: menu.setPriority(-20, "-20") }
        QQC2.MenuItem { text: qsTr("较高 (-10)"); onTriggered: menu.setPriority(-10, "-10") }
        QQC2.MenuItem { text: qsTr("普通 (0)"); onTriggered: menu.setPriority(0, "0") }
        QQC2.MenuItem { text: qsTr("较低 (10)"); onTriggered: menu.setPriority(10, "10") }
        QQC2.MenuItem { text: qsTr("最低 (19)"); onTriggered: menu.setPriority(19, "19") }
    }
}
