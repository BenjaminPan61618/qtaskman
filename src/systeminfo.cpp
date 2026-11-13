#include "systeminfo.h"

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QSysInfo>
#include <QThread>

namespace {

QStringList readAllLines(QFile &file)
{
    QStringList lines;
    const QByteArray data = file.readAll();
    int start = 0;
    while (start < data.size()) {
        int end = data.indexOf('\n', start);
        if (end < 0) {
            end = data.size();
        }
        lines.append(QString::fromUtf8(data.mid(start, end - start)));
        start = end + 1;
    }
    return lines;
}

QString prettyDesktopName(const QString &raw)
{
    const QString upper = raw.toUpper();
    if (upper.contains(QLatin1String("KDE")) || upper.contains(QLatin1String("PLASMA"))) {
        return QStringLiteral("KDE Plasma");
    }
    if (upper.contains(QLatin1String("GNOME"))) {
        return QStringLiteral("GNOME");
    }
    if (upper.contains(QLatin1String("XFCE"))) {
        return QStringLiteral("Xfce");
    }
    if (upper.contains(QLatin1String("CINNAMON"))) {
        return QStringLiteral("Cinnamon");
    }
    if (upper.contains(QLatin1String("MATE"))) {
        return QStringLiteral("MATE");
    }
    if (upper.contains(QLatin1String("LXQT"))) {
        return QStringLiteral("LXQt");
    }
    if (upper.contains(QLatin1String("LXDE"))) {
        return QStringLiteral("LXDE");
    }
    if (upper.contains(QLatin1String("SWAY"))) {
        return QStringLiteral("Sway");
    }
    if (upper.contains(QLatin1String("HYPRLAND"))) {
        return QStringLiteral("Hyprland");
    }
    if (upper.isEmpty()) {
        return {};
    }
    QString first = raw.section(QLatin1Char(':'), 0, 0);
    if (first.isEmpty()) {
        return raw;
    }
    first[0] = first.at(0).toUpper();
    return first;
}

} // namespace

SystemInfo::SystemInfo(QObject *parent)
    : QObject(parent)
{
    m_distro = osReleaseValue(QStringLiteral("PRETTY_NAME"));
    if (m_distro.isEmpty()) {
        const QString name = osReleaseValue(QStringLiteral("NAME"));
        const QString version = osReleaseValue(QStringLiteral("VERSION"));
        m_distro = (name + QLatin1Char(' ') + version).trimmed();
    }

    m_kernel = QSysInfo::kernelType() + QLatin1Char(' ') + QSysInfo::kernelVersion();
    m_arch = QSysInfo::currentCpuArchitecture();
    m_bits = QT_POINTER_SIZE == 8 ? QStringLiteral("64") : QStringLiteral("32");
    m_gpu = detectGpu();
    m_sessionType = detectSession();
    m_desktop = detectDesktop();
    m_hostname = QSysInfo::machineHostName();
    m_cpuModel = readCpuModel();
    m_cpuCores = QThread::idealThreadCount();

    QFile meminfo(QStringLiteral("/proc/meminfo"));
    if (meminfo.open(QIODevice::ReadOnly)) {
        for (const QString &line : readAllLines(meminfo)) {
            if (line.startsWith(QLatin1String("MemTotal:"))) {
                const qint64 bytes = line.section(QLatin1Char(':'), 1).trimmed().section(QLatin1Char(' '), 0, 0).toLongLong() * 1024;
                m_memory = humanBytes(bytes);
                break;
            }
        }
    }
    if (m_memory.isEmpty()) {
        m_memory.clear();
    }
}

QString SystemInfo::osReleaseValue(const QString &key)
{
    QFile file(QStringLiteral("/etc/os-release"));
    if (!file.open(QIODevice::ReadOnly)) {
        return {};
    }
    while (!file.atEnd()) {
        const QString line = QString::fromUtf8(file.readLine()).trimmed();
        if (line.startsWith(key + QLatin1Char('='))) {
            QString value = line.mid(key.size() + 1);
            if (value.size() >= 2 && value.startsWith(QLatin1Char('"')) && value.endsWith(QLatin1Char('"'))) {
                value = value.mid(1, value.size() - 2);
            }
            return value;
        }
    }
    return {};
}

QString SystemInfo::detectGpu()
{
    QProcess lspci;
    lspci.start(QStringLiteral("lspci"), {QStringLiteral("-nn")});
    if (lspci.waitForFinished(1500)) {
        QStringList controllers;
        const QString output = QString::fromLocal8Bit(lspci.readAllStandardOutput());
        const QStringList lines = output.split(QLatin1Char('\n'), Qt::SkipEmptyParts);
        for (const QString &line : lines) {
            if (!(line.contains(QLatin1String("VGA compatible controller"))
                  || line.contains(QLatin1String("3D controller"))
                  || line.contains(QLatin1String("Display controller")))) {
                continue;
            }
            const QString description = line.section(QLatin1Char(':'), 2).trimmed();
            controllers << description;
        }
        if (!controllers.isEmpty()) {
            return controllers.join(QStringLiteral(" / "));
        }
    }

    const QDir drm(QStringLiteral("/sys/class/drm"));
    const QStringList cards = drm.entryList(QStringList{QStringLiteral("card[0-9]*")}, QDir::Dirs);
    for (const QString &card : cards) {
        const QString ueventPath = drm.filePath(card + QStringLiteral("/device/uevent"));
        QFile uevent(ueventPath);
        if (!uevent.open(QIODevice::ReadOnly)) {
            continue;
        }
        QString driver;
        QString pciId;
        for (const QString &line : readAllLines(uevent)) {
            if (line.startsWith(QLatin1String("DRIVER="))) {
                driver = line.mid(7);
            } else if (line.startsWith(QLatin1String("PCI_ID="))) {
                pciId = line.mid(7);
            }
        }
        if (!pciId.isEmpty()) {
            if (driver.isEmpty()) {
                return QStringLiteral("PCI %1").arg(pciId);
            }
            return QStringLiteral("PCI %1 (%2)").arg(pciId, driver);
        }
    }
    return {};
}

QString SystemInfo::detectDesktop()
{
    QString desktop = qEnvironmentVariable("XDG_CURRENT_DESKTOP");
    if (desktop.isEmpty()) {
        desktop = qEnvironmentVariable("DESKTOP_SESSION");
    }
    QString pretty = prettyDesktopName(desktop);
    const QString kdeVersion = qEnvironmentVariable("KDE_SESSION_VERSION");
    if (pretty == QLatin1String("KDE Plasma") && !kdeVersion.isEmpty()) {
        pretty += QLatin1Char(' ') + kdeVersion;
    }
    return pretty;
}

QString SystemInfo::detectSession()
{
    const QString type = qEnvironmentVariable("XDG_SESSION_TYPE").toLower();
    if (type == QLatin1String("wayland")) {
        return QStringLiteral("Wayland");
    }
    if (type == QLatin1String("x11")) {
        return QStringLiteral("X11");
    }
    if (!qEnvironmentVariable("WAYLAND_DISPLAY").isEmpty()) {
        return QStringLiteral("Wayland");
    }
    if (!qEnvironmentVariable("DISPLAY").isEmpty()) {
        return QStringLiteral("X11");
    }
    return {};
}

QString SystemInfo::readCpuModel()
{
    QFile file(QStringLiteral("/proc/cpuinfo"));
    if (!file.open(QIODevice::ReadOnly)) {
        return {};
    }
    for (const QString &line : readAllLines(file)) {
        if (line.startsWith(QLatin1String("model name"))) {
            const int colon = line.indexOf(QLatin1Char(':'));
            if (colon >= 0) {
                return line.mid(colon + 1).trimmed();
            }
        }
    }
    return {};
}

QString SystemInfo::humanBytes(qint64 bytes)
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
