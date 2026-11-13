#include "systemmonitor.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QStorageInfo>
#include <QTextStream>

#include <algorithm>

namespace {

QString humanBytes(double bytes)
{
    static const char *units[] = {"B", "KiB", "MiB", "GiB", "TiB"};
    double value = bytes;
    int unit = 0;
    while (value >= 1024.0 && unit < 4) {
        value /= 1024.0;
        ++unit;
    }
    const int precision = value >= 100 ? 0 : (value >= 10 ? 1 : 2);
    return QString::number(value, 'f', precision) + QLatin1Char(' ') + QLatin1String(units[unit]);
}

QList<QByteArray> readAllLines(QFile &file)
{
    QList<QByteArray> lines;
    const QByteArray data = file.readAll();
    int start = 0;
    while (start < data.size()) {
        int end = data.indexOf('\n', start);
        if (end < 0) {
            end = data.size();
        }
        lines.append(data.mid(start, end - start));
        start = end + 1;
    }
    return lines;
}

bool isPhysicalDisk(const QString &name)
{
    static const QRegularExpression re(
        QStringLiteral("^(sd[a-z]+|nvme\\d+n\\d+|vd[a-z]+|xvd[a-z]+|mmcblk\\d+|md\\d+)$"));
    return re.match(name).hasMatch();
}

bool isPseudoFilesystem(const QString &type)
{
    static const QSet<QString> pseudo = {
        QStringLiteral("tmpfs"), QStringLiteral("devtmpfs"), QStringLiteral("proc"),
        QStringLiteral("sysfs"), QStringLiteral("cgroup2"), QStringLiteral("cgroup"),
        QStringLiteral("devpts"), QStringLiteral("securityfs"), QStringLiteral("debugfs"),
        QStringLiteral("tracefs"), QStringLiteral("mqueue"), QStringLiteral("hugetlbfs"),
        QStringLiteral("pstore"), QStringLiteral("bpf"), QStringLiteral("configfs"),
        QStringLiteral("fusectl"), QStringLiteral("autofs"), QStringLiteral("binfmt_misc"),
        QStringLiteral("efivarfs"), QStringLiteral("ramfs"), QStringLiteral("overlay"),
        QStringLiteral("squashfs"), QStringLiteral("nsfs"), QStringLiteral("rpc_pipefs"),
    };
    return pseudo.contains(type);
}

} // namespace

SystemMonitor::SystemMonitor(QObject *parent)
    : QObject(parent)
{
    m_timer.setInterval(1000);
    connect(&m_timer, &QTimer::timeout, this, &SystemMonitor::sample);
    sample();
    m_timer.start();
}

void SystemMonitor::setSampleInterval(int ms)
{
    if (ms < 500) {
        ms = 500;
    } else if (ms > 10000) {
        ms = 10000;
    }
    m_timer.setInterval(ms);
}

void SystemMonitor::refresh()
{
    sample();
}

void SystemMonitor::clearHistory()
{
    m_cpuHistory.clear();
    m_memHistory.clear();
    m_netUpHistory.clear();
    m_netDownHistory.clear();
    m_diskReadHistory.clear();
    m_diskWriteHistory.clear();
    m_cpuTempHistory.clear();
    m_memTempHistory.clear();
    emit statsChanged();
}

QString SystemMonitor::exportReport()
{
    const QString now = QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd_HH-mm-ss"));
    QString dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (dir.isEmpty()) {
        dir = QDir::homePath();
    }
    const QString path = dir + QStringLiteral("/qtaskman_report_") + now + QStringLiteral(".txt");

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return QString();
    }

    QTextStream out(&file);
    out << "qTaskMan — " << tr("系统监控报告") << "\n";
    out << "========================================\n";
    out << tr("生成时间") << ": " << QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss")) << "\n";
    out << tr("系统运行") << ": " << formatUptime(m_uptimeSeconds) << "\n\n";

    out << "--- " << tr("CPU") << " ---\n";
    out << tr("使用率") << ": " << formatPercent(m_cpuPercent) << "\n";
    out << tr("核心数") << ": " << m_perCore.size() << "\n";
    for (int i = 0; i < m_perCore.size(); ++i) {
        out << "  " << tr("核心 %1").arg(i) << ": " << formatPercent(m_perCore[i].toDouble()) << "\n";
    }
    out << tr("温度") << ": " << (m_cpuTemp >= 0 ? QString::number(m_cpuTemp, 'f', 1) + QStringLiteral(" °C") : tr("不可用")) << "\n\n";

    out << "--- " << tr("内存") << " ---\n";
    out << tr("已用") << ": " << humanBytes(m_memUsed) << " / " << humanBytes(m_memTotal)
        << " (" << formatPercent(m_memPercent) << ")\n";
    out << tr("Swap") << ": " << humanBytes(m_swapUsed) << " / " << humanBytes(m_swapTotal) << "\n";
    out << tr("温度") << ": " << (m_memTemp >= 0 ? QString::number(m_memTemp, 'f', 1) + QStringLiteral(" °C") : tr("不可用")) << "\n\n";

    out << "--- " << tr("网络") << " ---\n";
    out << tr("上传") << ": " << formatRate(m_netUp) << "\n";
    out << tr("下载") << ": " << formatRate(m_netDown) << "\n";
    for (const QVariant &v : m_interfaces) {
        const QVariantMap iface = v.toMap();
        out << "  " << iface.value(QStringLiteral("name")).toString()
            << ": " << tr("上传 %1 / 下载 %2")
                   .arg(formatRate(iface.value(QStringLiteral("up")).toDouble()))
                   .arg(formatRate(iface.value(QStringLiteral("down")).toDouble())) << "\n";
    }
    out << "\n";

    out << "--- " << tr("磁盘") << " ---\n";
    out << tr("读取") << ": " << formatRate(m_diskRead) << "\n";
    out << tr("写入") << ": " << formatRate(m_diskWrite) << "\n";
    for (const QVariant &v : m_mounts) {
        const QVariantMap mount = v.toMap();
        out << "  " << mount.value(QStringLiteral("mount")).toString()
            << " (" << mount.value(QStringLiteral("name")).toString() << "): "
            << humanBytes(mount.value(QStringLiteral("used")).toDouble()) << " / "
            << humanBytes(mount.value(QStringLiteral("total")).toDouble()) << " ("
            << formatPercent(mount.value(QStringLiteral("percent")).toDouble()) << ")\n";
    }
    out << "\n";

    out << "--- " << tr("电池") << " ---\n";
    if (m_batteryPercent >= 0) {
        out << tr("电量") << ": " << formatPercent(m_batteryPercent) << "\n";
        out << tr("状态") << ": " << m_batteryState << "\n";
        out << tr("速率") << ": " << formatRate(m_batteryRate) << "\n";
    } else {
        out << tr("不可用") << "\n";
    }
    out << "\n";

    out << "--- " << tr("温度传感器") << " ---\n";
    for (const QVariant &v : m_sensors) {
        const QVariantMap sensor = v.toMap();
        out << "  " << sensor.value(QStringLiteral("label")).toString() << ": "
            << QString::number(sensor.value(QStringLiteral("temp")).toDouble(), 'f', 1) << " °C\n";
    }

    file.close();
    return path;
}

void SystemMonitor::pushHistory(QVariantList &history, double value)
{
    history.append(value);
    while (history.size() > 60) {
        history.removeFirst();
    }
}

QString SystemMonitor::formatBytes(double bytes) const
{
    return humanBytes(bytes);
}

QString SystemMonitor::formatRate(double bytesPerSecond) const
{
    return humanBytes(bytesPerSecond) + QStringLiteral("/s");
}

QString SystemMonitor::formatPercent(double value, int decimals) const
{
    return QString::number(value, 'f', decimals) + QLatin1Char('%');
}

QString SystemMonitor::formatUptime(qint64 seconds) const
{
    const qint64 days = seconds / 86400;
    const qint64 hours = (seconds % 86400) / 3600;
    const qint64 minutes = (seconds % 3600) / 60;
    if (days > 0) {
        return tr("%1 天 %2 小时").arg(days).arg(hours);
    }
    if (hours > 0) {
        return tr("%1 小时 %2 分钟").arg(hours).arg(minutes);
    }
    return tr("%1 分钟").arg(minutes);
}

void SystemMonitor::sample()
{
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    double seconds = m_lastSampleMs > 0 ? double(now - m_lastSampleMs) / 1000.0 : 1.0;
    seconds = qBound(0.05, seconds, 30.0);
    m_lastSampleMs = now;

    sampleCpu();
    sampleMemory();
    sampleNetwork(seconds);
    sampleDisk(seconds);
    sampleMounts();
    sampleTemperatures();
    sampleBattery();
    sampleUptime();

    pushHistory(m_cpuHistory, m_cpuPercent);
    pushHistory(m_memHistory, m_memPercent);
    pushHistory(m_netUpHistory, m_netUp);
    pushHistory(m_netDownHistory, m_netDown);
    pushHistory(m_diskReadHistory, m_diskRead);
    pushHistory(m_diskWriteHistory, m_diskWrite);
    pushHistory(m_cpuTempHistory, std::max(0.0, m_cpuTemp));
    pushHistory(m_memTempHistory, std::max(0.0, m_memTemp));

    emit statsChanged();
}

void SystemMonitor::sampleCpu()
{
    QFile file(QStringLiteral("/proc/stat"));
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QVector<qint64> busy;
    QVector<qint64> total;
    for (const QByteArray &line : readAllLines(file)) {
        if (!line.startsWith("cpu")) {
            continue;
        }
        const QList<QByteArray> tokens = line.simplified().split(' ');
        qint64 sum = 0;
        qint64 idle = 0;
        for (int i = 1; i < tokens.size(); ++i) {
            const qint64 value = tokens.at(i).toLongLong();
            sum += value;
            if (i == 4 || i == 5) {
                idle += value;
            }
        }
        if (sum <= 0) {
            continue;
        }
        busy.append(sum - idle);
        total.append(sum);
    }

    if (total.isEmpty()) {
        return;
    }

    if (m_prevCpuTotal.size() != total.size()) {
        m_prevCpuBusy = busy;
        m_prevCpuTotal = total;
        m_cpuPercent = 0.0;
        m_perCore.clear();
        for (int i = 1; i < total.size(); ++i) {
            m_perCore.append(0.0);
        }
        return;
    }

    auto percent = [](qint64 busyNow, qint64 busyPrev, qint64 totalNow, qint64 totalPrev) {
        const qint64 deltaBusy = busyNow - busyPrev;
        const qint64 deltaTotal = totalNow - totalPrev;
        return deltaTotal > 0 ? double(deltaBusy) * 100.0 / double(deltaTotal) : 0.0;
    };

    m_cpuPercent = percent(busy.at(0), m_prevCpuBusy.at(0), total.at(0), m_prevCpuTotal.at(0));

    QVariantList cores;
    for (int i = 1; i < total.size(); ++i) {
        cores.append(percent(busy.at(i), m_prevCpuBusy.at(i), total.at(i), m_prevCpuTotal.at(i)));
    }
    m_perCore = cores;

    m_prevCpuBusy = busy;
    m_prevCpuTotal = total;
}

void SystemMonitor::sampleMemory()
{
    QFile file(QStringLiteral("/proc/meminfo"));
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    qint64 memTotal = 0;
    qint64 memAvailable = 0;
    qint64 swapTotal = 0;
    qint64 swapFree = 0;

    for (const QByteArray &line : readAllLines(file)) {
        const qint64 value = line.mid(line.indexOf(':') + 1).trimmed().split(' ').value(0).toLongLong() * 1024;
        if (line.startsWith("MemTotal:")) {
            memTotal = value;
        } else if (line.startsWith("MemAvailable:")) {
            memAvailable = value;
        } else if (line.startsWith("SwapTotal:")) {
            swapTotal = value;
        } else if (line.startsWith("SwapFree:")) {
            swapFree = value;
        }
    }

    m_memTotal = double(memTotal);
    m_memUsed = double(memTotal - memAvailable);
    m_memPercent = memTotal > 0 ? double(memTotal - memAvailable) * 100.0 / double(memTotal) : 0.0;
    m_swapTotal = double(swapTotal);
    m_swapUsed = double(swapTotal - swapFree);
}

void SystemMonitor::sampleNetwork(double seconds)
{
    QFile file(QStringLiteral("/proc/net/dev"));
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QVariantList interfaces;
    double totalUp = 0.0;
    double totalDown = 0.0;

    int lineIndex = 0;
    for (const QByteArray &line : readAllLines(file)) {
        if (++lineIndex <= 2) {
            continue;
        }
        const int colon = line.indexOf(':');
        if (colon < 0) {
            continue;
        }
        const QString name = QString::fromLocal8Bit(line.left(colon)).trimmed();
        const QList<QByteArray> fields = line.mid(colon + 1).simplified().split(' ');
        if (fields.size() < 9) {
            continue;
        }
        const qint64 rx = fields.at(0).toLongLong();
        const qint64 tx = fields.at(8).toLongLong();

        double up = 0.0;
        double down = 0.0;
        if (m_prevNetRx.contains(name)) {
            up = double(qMax<qint64>(0, tx - m_prevNetTx.value(name))) / seconds;
            down = double(qMax<qint64>(0, rx - m_prevNetRx.value(name))) / seconds;
        }
        m_prevNetRx.insert(name, rx);
        m_prevNetTx.insert(name, tx);

        if (name == QLatin1String("lo")) {
            continue;
        }
        totalUp += up;
        totalDown += down;
        if (up > 0.0 || down > 0.0 || name.startsWith(QLatin1String("en")) || name.startsWith(QLatin1String("wl"))
            || name.startsWith(QLatin1String("eth"))) {
            interfaces.append(QVariantMap{
                {QStringLiteral("name"), name},
                {QStringLiteral("up"), up},
                {QStringLiteral("down"), down},
            });
        }
    }

    m_netUp = totalUp;
    m_netDown = totalDown;
    m_interfaces = interfaces;
}

void SystemMonitor::sampleDisk(double seconds)
{
    QFile file(QStringLiteral("/proc/diskstats"));
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    qint64 readBytes = 0;
    qint64 writeBytes = 0;
    for (const QByteArray &line : readAllLines(file)) {
        const QList<QByteArray> tokens = line.simplified().split(' ');
        if (tokens.size() < 10) {
            continue;
        }
        const QString name = QString::fromLocal8Bit(tokens.at(2));
        if (!isPhysicalDisk(name)) {
            continue;
        }
        const qint64 sectorsRead = tokens.at(5).toLongLong();
        const qint64 sectorsWritten = tokens.at(9).toLongLong();

        if (m_prevDiskRead.contains(name)) {
            readBytes += qMax<qint64>(0, sectorsRead - m_prevDiskRead.value(name));
            writeBytes += qMax<qint64>(0, sectorsWritten - m_prevDiskWrite.value(name));
        }
        m_prevDiskRead.insert(name, sectorsRead);
        m_prevDiskWrite.insert(name, sectorsWritten);
    }

    m_diskRead = double(readBytes) * 512.0 / seconds;
    m_diskWrite = double(writeBytes) * 512.0 / seconds;
}

void SystemMonitor::sampleMounts()
{
    QVariantList mounts;
    const QList<QStorageInfo> volumes = QStorageInfo::mountedVolumes();
    for (const QStorageInfo &volume : volumes) {
        if (!volume.isValid() || !volume.isReady() || volume.bytesTotal() <= 0) {
            continue;
        }
        if (isPseudoFilesystem(QString::fromLocal8Bit(volume.fileSystemType()))) {
            continue;
        }
        const QVariantMap entry{
            {QStringLiteral("name"), volume.displayName().isEmpty()
                 ? volume.rootPath() : volume.displayName()},
            {QStringLiteral("mount"), volume.rootPath()},
            {QStringLiteral("used"), double(volume.bytesTotal() - volume.bytesAvailable())},
            {QStringLiteral("total"), double(volume.bytesTotal())},
            {QStringLiteral("percent"),
             double(volume.bytesTotal() - volume.bytesAvailable()) * 100.0 / double(volume.bytesTotal())},
        };
        mounts.append(entry);
    }

    std::sort(mounts.begin(), mounts.end(), [](const QVariant &a, const QVariant &b) {
        const QString pathA = a.toMap().value(QStringLiteral("mount")).toString();
        const QString pathB = b.toMap().value(QStringLiteral("mount")).toString();
        if (pathA == QLatin1String("/")) {
            return true;
        }
        if (pathB == QLatin1String("/")) {
            return false;
        }
        return pathA < pathB;
    });

    if (mounts.size() > 8) {
        mounts = mounts.mid(0, 8);
    }
    m_mounts = mounts;
}

void SystemMonitor::sampleTemperatures()
{
    QVariantList sensors;
    double cpuTemp = -1.0;
    double memTemp = -1.0;

    const QDir hwmon(QStringLiteral("/sys/class/hwmon"));
    const QStringList entries = hwmon.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &entry : entries) {
        const QString base = hwmon.filePath(entry);
        QFile nameFile(base + QStringLiteral("/name"));
        QString chipName;
        if (nameFile.open(QIODevice::ReadOnly)) {
            chipName = QString::fromUtf8(nameFile.readLine()).trimmed();
        }

        const QStringList tempFiles = QDir(base).entryList(QStringList{QStringLiteral("temp*_input")}, QDir::Files);
        for (const QString &tempFile : tempFiles) {
            QFile input(base + QLatin1Char('/') + tempFile);
            if (!input.open(QIODevice::ReadOnly)) {
                continue;
            }
            const double celsius = QString::fromUtf8(input.readLine()).trimmed().toDouble() / 1000.0;
            if (celsius <= -50.0 || celsius >= 200.0) {
                continue;
            }

            QString label = chipName;
            QString labelName = tempFile;
            labelName.replace(QStringLiteral("_input"), QStringLiteral("_label"));
            const QString labelPath = base + QLatin1Char('/') + labelName;
            QFile labelFile(labelPath);
            if (labelFile.open(QIODevice::ReadOnly)) {
                const QString text = QString::fromUtf8(labelFile.readLine()).trimmed();
                if (!text.isEmpty()) {
                    label = chipName + QStringLiteral(" / ") + text;
                }
            }

            QString kind = QStringLiteral("other");
            const QString lower = chipName.toLower();
            if (lower.contains(QLatin1String("coretemp")) || lower.contains(QLatin1String("k10temp"))
                || lower.contains(QLatin1String("zenpower")) || lower.contains(QLatin1String("cpu"))) {
                kind = QStringLiteral("cpu");
                cpuTemp = std::max(cpuTemp, celsius);
            } else if (lower.contains(QLatin1String("spd")) || lower.contains(QLatin1String("dimm"))
                       || lower.contains(QLatin1String("jc42"))) {
                kind = QStringLiteral("memory");
                memTemp = std::max(memTemp, celsius);
            } else if (lower.contains(QLatin1String("nvme"))) {
                kind = QStringLiteral("nvme");
            }

            sensors.append(QVariantMap{
                {QStringLiteral("label"), label},
                {QStringLiteral("kind"), kind},
                {QStringLiteral("temp"), celsius},
            });
        }
    }

    m_sensors = sensors;
    m_cpuTemp = cpuTemp;
    m_memTemp = memTemp;
}

void SystemMonitor::sampleBattery()
{
    m_batteryPercent = -1.0;
    m_batteryRate = -1.0;
    m_batteryState.clear();

    const QDir power(QStringLiteral("/sys/class/power_supply"));
    const QStringList entries = power.entryList(QStringList{QStringLiteral("BAT*")}, QDir::Dirs | QDir::NoDotAndDotDot);
    if (entries.isEmpty()) {
        return;
    }

    const QString base = power.filePath(entries.first());
    QFile capacity(base + QStringLiteral("/capacity"));
    if (capacity.open(QIODevice::ReadOnly)) {
        m_batteryPercent = QString::fromUtf8(capacity.readLine()).trimmed().toDouble();
    }
    QFile status(base + QStringLiteral("/status"));
    if (status.open(QIODevice::ReadOnly)) {
        m_batteryState = QString::fromUtf8(status.readLine()).trimmed();
    }
    QFile powerNow(base + QStringLiteral("/power_now"));
    if (powerNow.open(QIODevice::ReadOnly)) {
        const double microwatts = QString::fromUtf8(powerNow.readLine()).trimmed().toDouble();
        if (microwatts > 0.0) {
            m_batteryRate = microwatts / 1000000.0;
        }
    }
}

void SystemMonitor::sampleUptime()
{
    QFile file(QStringLiteral("/proc/uptime"));
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    m_uptimeSeconds = qint64(QString::fromUtf8(file.readLine()).section(QLatin1Char(' '), 0, 0).toDouble());
}
