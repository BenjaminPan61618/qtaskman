#include "processcontroller.h"

#include <cerrno>
#include <cstring>

#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QUrl>

#include <signal.h>
#include <sys/resource.h>

ProcessController::ProcessController(QObject *parent)
    : QObject(parent)
{
}

void ProcessController::fail(const QString &message)
{
    m_lastError = message;
    emit lastErrorChanged();
}

QString ProcessController::executablePath(int pid) const
{
    if (pid <= 0) {
        return {};
    }
    const QString link = QStringLiteral("/proc/%1/exe").arg(pid);
    QString path = QFile::symLinkTarget(link);
    // Deleted executables show up as "/path (deleted)".
    if (path.endsWith(QStringLiteral(" (deleted)"))) {
        path.chop(10);
    }
    return path;
}

bool ProcessController::openExecutableLocation(int pid)
{
    const QString exe = executablePath(pid);
    if (exe.isEmpty()) {
        fail(tr("无法获取可执行文件路径"));
        return false;
    }
    const QString dir = QFileInfo(exe).absolutePath();
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(dir))) {
        fail(tr("无法打开文件管理器"));
        return false;
    }
    clearError();
    return true;
}

bool ProcessController::restartProcess(int pid)
{
    if (pid <= 0) {
        fail(tr("无效的进程号"));
        return false;
    }

    const QString base = QStringLiteral("/proc/%1").arg(pid);
    const QString exe = executablePath(pid);
    if (exe.isEmpty()) {
        fail(tr("无法获取可执行文件路径"));
        return false;
    }

    QStringList args;
    QFile cmdFile(base + QStringLiteral("/cmdline"));
    if (cmdFile.open(QIODevice::ReadOnly)) {
        const QList<QByteArray> parts = cmdFile.readAll().split('\0');
        for (int i = 1; i < parts.size(); ++i) {
            if (!parts.at(i).isEmpty()) {
                args.append(QString::fromLocal8Bit(parts.at(i)));
            }
        }
    }

    const QString cwdLink = QFile::symLinkTarget(base + QStringLiteral("/cwd"));
    const QString cwd = cwdLink.isEmpty() ? QDir::currentPath() : cwdLink;

    if (::kill(pid_t(pid), SIGTERM) != 0) {
        fail(tr("结束进程失败：%1").arg(QString::fromLocal8Bit(std::strerror(errno))));
        return false;
    }

    if (!QProcess::startDetached(exe, args, cwd)) {
        fail(tr("重新启动进程失败"));
        return false;
    }
    clearError();
    return true;
}

void ProcessController::clearError()
{
    if (m_lastError.isEmpty()) {
        return;
    }
    m_lastError.clear();
    emit lastErrorChanged();
}

bool ProcessController::sendSignal(int pid, int signalNumber)
{
    if (pid <= 0) {
        fail(tr("无效的进程号"));
        return false;
    }
    if (::kill(pid_t(pid), signalNumber) != 0) {
        fail(tr("发送信号失败：%1").arg(QString::fromLocal8Bit(std::strerror(errno))));
        return false;
    }
    clearError();
    return true;
}

bool ProcessController::setPriority(int pid, int niceValue)
{
    if (pid <= 0) {
        fail(tr("无效的进程号"));
        return false;
    }
    errno = 0;
    if (::setpriority(PRIO_PROCESS, id_t(pid), niceValue) != 0) {
        fail(tr("设置优先级失败：%1").arg(QString::fromLocal8Bit(std::strerror(errno))));
        return false;
    }
    clearError();
    return true;
}
