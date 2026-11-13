#include "appsmodel.h"

#include "processsampler.h"
#include "windowprobe.h"

#include <QCollator>
#include <QFile>
#include <QLocale>
#include <algorithm>

AppsModel::AppsModel(ProcessSampler *sampler, WindowProbe *probe, QObject *parent)
    : QAbstractListModel(parent)
    , m_sampler(sampler)
    , m_probe(probe)
{
    connect(m_probe, &WindowProbe::windowsChanged, this, &AppsModel::rebuild);
    connect(m_probe, &WindowProbe::availableChanged, this, &AppsModel::rebuild);
    connect(m_sampler, &ProcessSampler::sampled, this, &AppsModel::onSampled);
    rebuild();
}

int AppsModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : int(m_rows.size());
}

QVariant AppsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_rows.size()) {
        return {};
    }
    const AppRow &row = m_rows.at(index.row());
    const auto &procs = m_sampler->processes();
    const auto it = procs.constFind(row.pid);
    static const ProcMetrics empty;
    const ProcMetrics &metrics = it != procs.constEnd() ? it.value() : empty;

    switch (role) {
    case PidRole:
        return row.pid;
    case AppNameRole:
        return row.name;
    case TitleRole:
        return row.title;
    case IconNameRole:
        return row.icon.isEmpty() ? metrics.iconName : row.icon;
    case CpuRole:
        return metrics.cpuPercent;
    case MemRole:
        return double(metrics.rssBytes);
    case MemPercentRole:
        return metrics.memPercent;
    case NetUpRole:
        return metrics.netUp;
    case NetDownRole:
        return metrics.netDown;
    case DiskReadRole:
        return metrics.diskRead;
    case DiskWriteRole:
        return metrics.diskWrite;
    case ActiveWindowRole:
        return row.active;
    case PowerRole:
        return -1.0;
    case StateRole:
        return QString(metrics.state);
    case IsNewRole:
        return metrics.ageSeconds >= 0 && metrics.ageSeconds <= 10
               && metrics.state != QLatin1Char('Z');
    default:
        return {};
    }
}

QHash<int, QByteArray> AppsModel::roleNames() const
{
    return {
        {PidRole, "pid"},
        {AppNameRole, "appName"},
        {TitleRole, "title"},
        {IconNameRole, "iconName"},
        {CpuRole, "cpu"},
        {MemRole, "mem"},
        {MemPercentRole, "memPercent"},
        {NetUpRole, "netUp"},
        {NetDownRole, "netDown"},
        {DiskReadRole, "diskRead"},
        {DiskWriteRole, "diskWrite"},
        {ActiveWindowRole, "activeWindow"},
        {PowerRole, "powerWatts"},
        {StateRole, "state"},
        {IsNewRole, "isNew"},
    };
}

bool AppsModel::looksLikeGuiApp(int pid)
{
    const auto cached = m_guiCache.constFind(pid);
    if (cached != m_guiCache.constEnd()) {
        return *cached;
    }
    bool gui = false;
    QFile maps(QStringLiteral("/proc/%1/maps").arg(pid));
    if (maps.open(QIODevice::ReadOnly)) {
        const QByteArray data = maps.readAll();
        gui = data.contains("libwayland-client") || data.contains("libX11.so");
    }
    m_guiCache.insert(pid, gui);
    return gui;
}

QVector<AppsModel::AppRow> AppsModel::buildFromWindows() const
{
    QHash<int, AppRow> byPid;
    const auto &procs = m_sampler->processes();
    for (const WindowProbe::Window &window : m_probe->windows()) {
        auto procIt = procs.constFind(window.pid);
        if (procIt == procs.constEnd()) {
            continue;
        }
        AppRow &row = byPid[window.pid];
        row.pid = window.pid;
        if (row.title.isEmpty() || window.caption.size() > row.title.size()) {
            row.title = window.caption;
        }
        row.active = row.active || window.active;
        if (row.name.isEmpty()) {
            const QString desktopName = m_sampler->desktopNameForExe(procIt->exe);
            if (!desktopName.isEmpty()) {
                row.name = desktopName;
            } else if (!window.cls.isEmpty()) {
                QString cls = window.cls;
                if (cls.endsWith(QLatin1String(".desktop"))) {
                    cls.chop(8);
                }
                row.name = cls;
            } else if (!window.name.isEmpty()) {
                row.name = window.name;
            } else {
                row.name = procIt->name;
            }
            row.icon = procIt->iconName;
        }
    }

    QVector<AppRow> rows;
    rows.reserve(byPid.size());
    for (auto it = byPid.constBegin(); it != byPid.constEnd(); ++it) {
        rows.append(it.value());
    }
    std::sort(rows.begin(), rows.end(), [](const AppRow &a, const AppRow &b) {
        if (a.active != b.active) {
            return a.active;
        }
        return QString::localeAwareCompare(a.name, b.name) < 0;
    });
    return rows;
}

QVector<AppsModel::AppRow> AppsModel::buildFallback()
{
    QVector<AppRow> rows;
    m_guiCache.clear();
    const auto &procs = m_sampler->processes();
    for (auto it = procs.constBegin(); it != procs.constEnd(); ++it) {
        const ProcMetrics &metrics = it.value();
        if (metrics.uid != m_sampler->selfUid()) {
            continue;
        }
        if (metrics.name == QLatin1String("kwin_wayland") || metrics.name == QLatin1String("kwin_x11")) {
            continue;
        }
        if (metrics.exe.isEmpty() || !looksLikeGuiApp(it.key())) {
            continue;
        }
        AppRow row;
        row.pid = it.key();
        row.name = m_sampler->desktopNameForExe(metrics.exe);
        if (row.name.isEmpty()) {
            row.name = metrics.name;
        }
        row.icon = metrics.iconName;
        rows.append(row);
    }
    std::sort(rows.begin(), rows.end(), [](const AppRow &a, const AppRow &b) {
        return QString::localeAwareCompare(a.name, b.name) < 0;
    });
    if (rows.size() > 60) {
        rows.resize(60);
    }
    return rows;
}

void AppsModel::rebuild()
{
    QVector<AppRow> next;
    if (m_probe->available() && !m_probe->windows().isEmpty()) {
        next = buildFromWindows();
    } else {
        next = buildFallback();
    }

    bool changed = next.size() != m_rows.size();
    if (!changed) {
        for (int i = 0; i < next.size(); ++i) {
            const AppRow &a = next.at(i);
            const AppRow &b = m_rows.at(i);
            if (a.pid != b.pid || a.name != b.name || a.title != b.title || a.active != b.active) {
                changed = true;
                break;
            }
        }
    }
    if (!changed) {
        return;
    }

    beginResetModel();
    m_rows = next;
    endResetModel();
    emit countChanged();
}

void AppsModel::refreshValues()
{
    if (m_rows.isEmpty()) {
        return;
    }
    emit dataChanged(index(0, 0), index(int(m_rows.size()) - 1, 0));
}

void AppsModel::onSampled()
{
    if (m_probe->available() && !m_probe->windows().isEmpty()) {
        rebuild();
    } else if (!m_probe->available()) {
        const QSet<int> pids(m_sampler->processes().keyBegin(), m_sampler->processes().keyEnd());
        if (pids != m_fallbackPids) {
            m_fallbackPids = pids;
            rebuild();
            return;
        }
    }
    refreshValues();
}
