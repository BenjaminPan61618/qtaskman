#include "processsampler.h"

#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QIcon>
#include <QProcess>
#include <QRegularExpression>
#include <QStandardPaths>

#include <pwd.h>
#include <unistd.h>

namespace {

QString humanBytes(qint64 bytes)
{
    static const char *units[] = {"B", "KiB", "MiB", "GiB", "TiB"};
    double value = double(bytes);
    int unit = 0;
    while (value >= 1024.0 && unit < 4) {
        value /= 1024.0;
        ++unit;
    }
    const int precision = value >= 100 ? 0 : (value >= 10 ? 1 : 2);
    return QString::number(value, 'f', precision) + QLatin1Char(' ') + QLatin1String(units[unit]);
}

QString readFirstLine(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return {};
    }
    return QString::fromLocal8Bit(file.readLine()).trimmed();
}

void parseSocketTotals(const QByteArray &output, QHash<int, qint64> &sent, QHash<int, qint64> &received)
{
    int currentPid = -1;
    const QList<QByteArray> lines = output.split('\n');
    for (const QByteArray &rawLine : lines) {
        const QByteArray trimmed = rawLine.trimmed();
        if (trimmed.isEmpty()) {
            currentPid = -1;
            continue;
        }
        if (trimmed.startsWith("State") || trimmed.startsWith("Netid") || trimmed.startsWith("Recv-Q")) {
            continue;
        }

        const bool continuation = rawLine.startsWith('\t') || rawLine.startsWith(' ');
        if (continuation) {
            if (currentPid <= 0) {
                continue;
            }
            const QList<QByteArray> tokens = trimmed.simplified().split(' ');
            for (const QByteArray &token : tokens) {
                if (token.startsWith("bytes_sent:")) {
                    sent[currentPid] += token.mid(11).toLongLong();
                } else if (token.startsWith("bytes_received:")) {
                    received[currentPid] += token.mid(15).toLongLong();
                }
            }
            continue;
        }

        const int pidPos = trimmed.indexOf("pid=");
        if (pidPos < 0) {
            currentPid = -1;
            continue;
        }
        int start = pidPos + 4;
        int end = start;
        while (end < trimmed.size() && trimmed.at(end) >= '0' && trimmed.at(end) <= '9') {
            ++end;
        }
        currentPid = trimmed.mid(start, end - start).toInt();
    }
}

} // namespace

ProcessSampler::ProcessSampler(QObject *parent)
    : QObject(parent)
{
    m_selfUid = ::getuid();
    m_ticksPerSecond = int(::sysconf(_SC_CLK_TCK));
    if (m_ticksPerSecond <= 0) {
        m_ticksPerSecond = 100;
    }

    QFile meminfo(QStringLiteral("/proc/meminfo"));
    if (meminfo.open(QIODevice::ReadOnly)) {
        const QByteArray data = meminfo.readAll();
        for (const QByteArray &rawLine : data.split('\n')) {
            if (rawLine.startsWith("MemTotal:")) {
                m_totalMemory = rawLine.mid(9).trimmed().split(' ').value(0).toLongLong() * 1024;
                break;
            }
        }
    }

    loadDesktopEntries();

    m_timer.setInterval(1000);
    connect(&m_timer, &QTimer::timeout, this, &ProcessSampler::sample);
}

int ProcessSampler::selfPid() const
{
    return int(::getpid());
}

void ProcessSampler::start(int intervalMs)
{
    m_timer.setInterval(intervalMs);
    m_lastSampleMs = 0;
    m_lastNetMs = 0;
    sample();
    m_timer.start();
}

void ProcessSampler::stop()
{
    m_timer.stop();
}

void ProcessSampler::sampleNow()
{
    sample();
}

void ProcessSampler::loadDesktopEntries()
{
    if (m_desktopLoaded) {
        return;
    }
    m_desktopLoaded = true;

    const QStringList roots = {
        QStringLiteral("/usr/share/applications"),
        QStringLiteral("/usr/local/share/applications"),
        QDir::homePath() + QStringLiteral("/.local/share/applications"),
    };

    for (const QString &root : roots) {
        QDirIterator it(root, QStringList{QStringLiteral("*.desktop")}, QDir::Files,
                        QDirIterator::Subdirectories);
        while (it.hasNext()) {
            const QString path = it.next();
            QFile file(path);
            if (!file.open(QIODevice::ReadOnly)) {
                continue;
            }
            bool inEntry = false;
            bool skip = false;
            QString exec;
            QString icon;
            QString name;
            while (!file.atEnd()) {
                const QString line = QString::fromUtf8(file.readLine()).trimmed();
                if (line.startsWith('[')) {
                    inEntry = line == QLatin1String("[Desktop Entry]");
                    continue;
                }
                if (!inEntry || line.isEmpty() || line.startsWith('#')) {
                    continue;
                }
                const int eq = line.indexOf('=');
                if (eq <= 0) {
                    continue;
                }
                const QString key = line.left(eq);
                const QString value = line.mid(eq + 1);
                if (key == QLatin1String("Exec")) {
                    exec = value;
                } else if (key == QLatin1String("Icon")) {
                    icon = value;
                } else if (key == QLatin1String("Name")) {
                    name = value;
                } else if (key == QLatin1String("NoDisplay") || key == QLatin1String("Hidden")) {
                    if (value == QLatin1String("true")) {
                        skip = true;
                    }
                }
            }
            if (skip || exec.isEmpty() || icon.isEmpty()) {
                continue;
            }

            QString program = exec.section(QLatin1Char(' '), 0, 0);
            if (program.endsWith(QLatin1Char('"')) || program.endsWith(QLatin1Char('\''))) {
                program.chop(1);
            }
            if (program.startsWith(QLatin1Char('"')) || program.startsWith(QLatin1Char('\''))) {
                program.remove(0, 1);
            }
            if (program == QLatin1String("env")) {
                program = exec.section(QLatin1Char(' '), 1, 1);
            }
            const QString base = QFileInfo(program).fileName();
            if (base.isEmpty() || m_desktopIcons.contains(base)) {
                continue;
            }
            m_desktopIcons.insert(base, icon);
            if (!name.isEmpty()) {
                m_desktopNames.insert(base, name);
            }
        }
    }
}

QString ProcessSampler::resolveIcon(const QString &exe)
{
    if (exe.isEmpty()) {
        return {};
    }
    QString base = QFileInfo(exe).fileName();
    if (base.endsWith(QStringLiteral(" (deleted)"))) {
        base.chop(10);
    }
    if (base.isEmpty()) {
        return {};
    }
    const auto cached = m_iconCache.constFind(base);
    if (cached != m_iconCache.constEnd()) {
        return *cached;
    }

    QString icon = m_desktopIcons.value(base);
    if (icon.isEmpty() && QIcon::hasThemeIcon(base)) {
        icon = base;
    }
    m_iconCache.insert(base, icon);
    return icon;
}

QString ProcessSampler::desktopNameForExe(const QString &exe) const
{
    if (exe.isEmpty()) {
        return {};
    }
    QString base = QFileInfo(exe).fileName();
    if (base.endsWith(QStringLiteral(" (deleted)"))) {
        base.chop(10);
    }
    return m_desktopNames.value(base);
}

void ProcessSampler::sample()
{
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    double seconds = m_lastSampleMs > 0 ? double(now - m_lastSampleMs) / 1000.0 : 1.0;
    seconds = qBound(0.05, seconds, 30.0);
    m_lastSampleMs = now;

    // Uptime is needed to turn /proc/<pid>/stat starttime into an age.
    {
        QFile uptimeFile(QStringLiteral("/proc/uptime"));
        if (uptimeFile.open(QIODevice::ReadOnly)) {
            m_uptimeSeconds = uptimeFile.readAll().split(' ').value(0).toDouble();
        }
    }

    QHash<int, ProcMetrics> next;
    next.reserve(m_processes.size() + 16);

    const QStringList entries = QDir(QStringLiteral("/proc")).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &entry : entries) {
        bool isPid = false;
        const int pid = entry.toInt(&isPid);
        if (!isPid) {
            continue;
        }

        const QString base = QStringLiteral("/proc/") + entry;

        QFile statFile(base + QStringLiteral("/stat"));
        if (!statFile.open(QIODevice::ReadOnly)) {
            continue;
        }
        const QByteArray statData = statFile.readAll();
        statFile.close();

        const int openParen = statData.indexOf('(');
        const int closeParen = statData.lastIndexOf(')');
        if (openParen < 0 || closeParen <= openParen) {
            continue;
        }

        ProcMetrics metrics;
        metrics.pid = pid;
        metrics.name = QString::fromLocal8Bit(statData.mid(openParen + 1, closeParen - openParen - 1));

        const QList<QByteArray> fields = statData.mid(closeParen + 2).split(' ');
        if (fields.size() < 22) {
            continue;
        }
        metrics.ppid = fields.value(1).toInt();
        metrics.cpuTicks = fields.value(11).toLongLong() + fields.value(12).toLongLong();
        metrics.nice = fields.value(16).toInt();

        const QByteArray stateField = fields.value(0);
        if (!stateField.isEmpty()) {
            metrics.state = QChar(stateField.at(0));
        }
        const qint64 startTicks = fields.value(19).toLongLong();
        if (startTicks > 0) {
            const double startSeconds = double(startTicks) / double(m_ticksPerSecond);
            metrics.ageSeconds = qMax<qint64>(0, qint64(m_uptimeSeconds - startSeconds));
        }

        // Memory / user information
        QFile statusFile(base + QStringLiteral("/status"));
        if (statusFile.open(QIODevice::ReadOnly)) {
            const QByteArray status = statusFile.readAll();
            statusFile.close();
            const QList<QByteArray> lines = status.split('\n');
            for (const QByteArray &line : lines) {
                if (line.startsWith("Uid:")) {
                    metrics.uid = pid_t(line.mid(4).trimmed().split('\t').value(0).toUInt());
                } else if (line.startsWith("VmRSS:")) {
                    metrics.rssBytes = line.mid(6).trimmed().split(' ').value(0).toLongLong() * 1024;
                }
            }
        }

        metrics.memPercent = m_totalMemory > 0 ? double(metrics.rssBytes) * 100.0 / double(m_totalMemory) : 0.0;

        struct passwd pwd;
        struct passwd *result = nullptr;
        QVarLengthArray<char, 1024> buffer(1024);
        if (::getpwuid_r(metrics.uid, &pwd, buffer.data(), buffer.size(), &result) == 0 && result) {
            metrics.user = QString::fromLocal8Bit(result->pw_name);
        }
        if (metrics.user.isEmpty()) {
            metrics.user = QString::number(metrics.uid);
        }

        // Disk IO
        QFile ioFile(base + QStringLiteral("/io"));
        if (ioFile.open(QIODevice::ReadOnly)) {
            const QList<QByteArray> lines = ioFile.readAll().split('\n');
            ioFile.close();
            for (const QByteArray &line : lines) {
                if (line.startsWith("read_bytes:")) {
                    metrics.diskReadTotal = line.mid(11).trimmed().toLongLong();
                } else if (line.startsWith("write_bytes:")) {
                    metrics.diskWriteTotal = line.mid(12).trimmed().toLongLong();
                }
            }
        }

        // Command line + executable
        QFile cmdlineFile(base + QStringLiteral("/cmdline"));
        if (cmdlineFile.open(QIODevice::ReadOnly)) {
            QByteArray cmd = cmdlineFile.readAll();
            cmd.replace('\0', ' ');
            metrics.cmdline = QString::fromLocal8Bit(cmd).trimmed();
            if (metrics.cmdline.size() > 400) {
                metrics.cmdline = metrics.cmdline.left(400) + QStringLiteral("…");
            }
        }
        metrics.exe = QFile::symLinkTarget(base + QStringLiteral("/exe"));
        if (metrics.exe.isEmpty()) {
            metrics.exe = readFirstLine(base + QStringLiteral("/comm"));
        }
        metrics.iconName = resolveIcon(metrics.exe);

        // Rate calculations
        const auto prevTicks = m_prevCpuTicks.constFind(pid);
        if (prevTicks != m_prevCpuTicks.constEnd() && metrics.cpuTicks >= *prevTicks) {
            metrics.cpuPercent = double(metrics.cpuTicks - *prevTicks) / double(m_ticksPerSecond) / seconds * 100.0;
        }
        m_prevCpuTicks.insert(pid, metrics.cpuTicks);

        const auto prevRead = m_prevDiskRead.constFind(pid);
        if (prevRead != m_prevDiskRead.constEnd() && metrics.diskReadTotal >= *prevRead) {
            metrics.diskRead = double(metrics.diskReadTotal - *prevRead) / seconds;
        }
        m_prevDiskRead.insert(pid, metrics.diskReadTotal);

        const auto prevWrite = m_prevDiskWrite.constFind(pid);
        if (prevWrite != m_prevDiskWrite.constEnd() && metrics.diskWriteTotal >= *prevWrite) {
            metrics.diskWrite = double(metrics.diskWriteTotal - *prevWrite) / seconds;
        }
        m_prevDiskWrite.insert(pid, metrics.diskWriteTotal);

        next.insert(pid, metrics);
    }

    m_processes = next;

    double netSeconds = m_lastNetMs > 0 ? double(now - m_lastNetMs) / 1000.0 : 1.0;
    netSeconds = qBound(0.05, netSeconds, 30.0);
    m_lastNetMs = now;
    sampleNetwork(netSeconds);

    // Drop bookkeeping for vanished processes
    for (auto it = m_prevCpuTicks.begin(); it != m_prevCpuTicks.end();) {
        it = m_processes.contains(it.key()) ? std::next(it) : m_prevCpuTicks.erase(it);
    }
    for (auto it = m_prevDiskRead.begin(); it != m_prevDiskRead.end();) {
        it = m_processes.contains(it.key()) ? std::next(it) : m_prevDiskRead.erase(it);
    }
    for (auto it = m_prevDiskWrite.begin(); it != m_prevDiskWrite.end();) {
        it = m_processes.contains(it.key()) ? std::next(it) : m_prevDiskWrite.erase(it);
    }
    for (auto it = m_prevNetSent.begin(); it != m_prevNetSent.end();) {
        it = m_processes.contains(it.key()) ? std::next(it) : m_prevNetSent.erase(it);
    }
    for (auto it = m_prevNetRecv.begin(); it != m_prevNetRecv.end();) {
        it = m_processes.contains(it.key()) ? std::next(it) : m_prevNetRecv.erase(it);
    }

    emit sampled();
}

void ProcessSampler::sampleNetwork(double seconds)
{
    QHash<int, qint64> sent;
    QHash<int, qint64> received;

    QProcess ss;
    ss.start(QStringLiteral("ss"), {QStringLiteral("-tinp")});
    if (ss.waitForFinished(800)) {
        parseSocketTotals(ss.readAllStandardOutput(), sent, received);
    } else {
        ss.kill();
        return;
    }

    for (auto it = sent.constBegin(); it != sent.constEnd(); ++it) {
        const int pid = it.key();
        auto proc = m_processes.find(pid);
        if (proc == m_processes.end()) {
            continue;
        }
        const qint64 prev = m_prevNetSent.value(pid, -1);
        if (prev >= 0 && it.value() >= prev) {
            proc->netUp = double(it.value() - prev) / seconds;
        }
        m_prevNetSent.insert(pid, it.value());
    }

    for (auto it = received.constBegin(); it != received.constEnd(); ++it) {
        const int pid = it.key();
        auto proc = m_processes.find(pid);
        if (proc == m_processes.end()) {
            continue;
        }
        const qint64 prev = m_prevNetRecv.value(pid, -1);
        if (prev >= 0 && it.value() >= prev) {
            proc->netDown = double(it.value() - prev) / seconds;
        }
        m_prevNetRecv.insert(pid, it.value());
    }
}
