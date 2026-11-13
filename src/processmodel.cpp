#include "processmodel.h"

#include "processsampler.h"

#include <QCollator>
#include <QDateTime>
#include <algorithm>

ProcessModel::ProcessModel(ProcessSampler *sampler, QObject *parent)
    : QAbstractListModel(parent)
    , m_sampler(sampler)
{
    connect(m_sampler, &ProcessSampler::sampled, this, &ProcessModel::onSampled);
}

int ProcessModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return int(m_rows.size());
}

QVariant ProcessModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_rows.size()) {
        return {};
    }
    const Row &row = m_rows.at(index.row());
    const Node &node = m_nodes[row.pid];
    const auto &procs = m_sampler->processes();
    const auto it = procs.constFind(row.pid);
    const ProcMetrics empty;
    const ProcMetrics &metrics = it != procs.constEnd() ? it.value() : empty;

    switch (role) {
    case PidRole:
        return row.pid;
    case PpidRole:
        return metrics.ppid;
    case NameRole:
        return metrics.name;
    case CommandRole:
        return metrics.cmdline;
    case UserRole:
        return metrics.user;
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
    case IconNameRole:
        return metrics.iconName;
    case DepthRole:
        return row.depth;
    case HasChildrenRole:
        return !node.children.isEmpty();
    case ExpandedRole:
        return m_expanded.contains(row.pid);
    case IsSelfRole:
        return row.pid == m_sampler->selfPid();
    case NiceRole:
        return metrics.nice;
    case StateRole:
        return QString(metrics.state);
    case IsNewRole:
        return metrics.ageSeconds >= 0 && metrics.ageSeconds <= 10
               && metrics.state != QLatin1Char('Z');
    default:
        return {};
    }
}

QHash<int, QByteArray> ProcessModel::roleNames() const
{
    return {
        {PidRole, "pid"},
        {PpidRole, "ppid"},
        {NameRole, "name"},
        {CommandRole, "command"},
        {UserRole, "user"},
        {CpuRole, "cpu"},
        {MemRole, "mem"},
        {MemPercentRole, "memPercent"},
        {NetUpRole, "netUp"},
        {NetDownRole, "netDown"},
        {DiskReadRole, "diskRead"},
        {DiskWriteRole, "diskWrite"},
        {IconNameRole, "iconName"},
        {DepthRole, "depth"},
        {HasChildrenRole, "hasChildren"},
        {ExpandedRole, "expanded"},
        {IsSelfRole, "isSelf"},
        {NiceRole, "niceValue"},
        {StateRole, "state"},
        {IsNewRole, "isNew"},
    };
}

void ProcessModel::setFilterMode(int mode)
{
    mode = qBound(0, mode, 3);
    if (m_filterMode == mode) {
        return;
    }
    m_filterMode = mode;
    emit filterModeChanged();
    m_expanded.clear();
    rebuild();
}

void ProcessModel::setFlatMode(bool enabled)
{
    if (m_flatMode == enabled) {
        return;
    }
    m_flatMode = enabled;
    emit flatModeChanged();
    updateFlat();
}

bool ProcessModel::matchesFilter(const ProcMetrics &metrics) const
{
    switch (m_filterMode) {
    case 0:
        return metrics.uid == m_sampler->selfUid();
    case 1:
        return metrics.uid >= 1000;
    case 2:
        return metrics.uid < 1000;
    default:
        return true;
    }
}

void ProcessModel::rebuild()
{
    const auto &procs = m_sampler->processes();

    m_nodes.clear();
    m_roots.clear();
    m_nodes.reserve(procs.size());

    for (auto it = procs.constBegin(); it != procs.constEnd(); ++it) {
        Node node;
        node.pid = it.key();
        node.parent = procs.contains(it.value().ppid) ? it.value().ppid : 0;
        m_nodes.insert(node.pid, node);
    }

    QSet<int> matching;
    for (auto it = procs.constBegin(); it != procs.constEnd(); ++it) {
        if (matchesFilter(it.value())) {
            matching.insert(it.key());
        }
    }

    // Re-parent every matching process to its nearest matching ancestor.
    for (const int pid : matching) {
        int parent = m_nodes.value(pid).parent;
        int guard = 0;
        while (parent > 0 && !matching.contains(parent) && guard++ < 4096) {
            parent = m_nodes.value(parent).parent;
        }
        if (parent > 0 && m_nodes.contains(parent)) {
            m_nodes[parent].children.append(pid);
        } else {
            m_roots.append(pid);
        }
    }

    auto sortPids = [](QVector<int> &pids) {
        std::sort(pids.begin(), pids.end());
    };
    sortPids(m_roots);
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        sortPids(it->children);
    }

    if (m_expanded.isEmpty()) {
        for (const int root : m_roots) {
            m_expanded.insert(root);
        }
    }

    m_knownPids = QSet<int>(procs.keyBegin(), procs.keyEnd());
    m_matching = matching;
    m_firstBuild = false;
    updateFlat();
}

void ProcessModel::updateFlat()
{
    QVector<Row> rows;
    rows.reserve(m_rows.size() + 32);

    if (m_flatMode) {
        QVector<int> pids(m_matching.begin(), m_matching.end());
        const auto &procs = m_sampler->processes();
        std::sort(pids.begin(), pids.end(), [&procs](int a, int b) {
            const int cmp = QString::localeAwareCompare(procs.value(a).name, procs.value(b).name);
            return cmp != 0 ? cmp < 0 : a < b;
        });
        for (const int pid : pids) {
            rows.append({pid, 0});
        }
    } else {
        QVector<QPair<int, int>> stack;
        for (int i = int(m_roots.size()) - 1; i >= 0; --i) {
            stack.append({m_roots.at(i), 0});
        }
        while (!stack.isEmpty()) {
            const auto [pid, depth] = stack.takeLast();
            rows.append({pid, depth});
            if (!m_expanded.contains(pid)) {
                continue;
            }
            const Node &node = m_nodes[pid];
            for (int i = int(node.children.size()) - 1; i >= 0; --i) {
                stack.append({node.children.at(i), depth + 1});
            }
        }
    }

    int start = 0;
    const int oldSize = int(m_rows.size());
    const int newSize = int(rows.size());
    while (start < oldSize && start < newSize && m_rows.at(start) == rows.at(start)) {
        ++start;
    }
    int oldEnd = oldSize;
    int newEnd = newSize;
    while (oldEnd > start && newEnd > start && m_rows.at(oldEnd - 1) == rows.at(newEnd - 1)) {
        --oldEnd;
        --newEnd;
    }

    if (oldEnd > start) {
        beginRemoveRows(QModelIndex(), start, oldEnd - 1);
        m_rows.remove(start, oldEnd - start);
        endRemoveRows();
    }
    if (newEnd > start) {
        beginInsertRows(QModelIndex(), start, newEnd - 1);
        for (int i = start; i < newEnd; ++i) {
            m_rows.insert(i, rows.at(i));
        }
        endInsertRows();
    }

    if (oldSize != newSize) {
        emit countChanged();
    }
}

void ProcessModel::onSampled()
{
    const auto &procs = m_sampler->processes();
    const QSet<int> pids(procs.keyBegin(), procs.keyEnd());
    if (pids != m_knownPids) {
        rebuild();
        return;
    }
    if (m_rows.isEmpty()) {
        return;
    }
    emit dataChanged(index(0, 0), index(int(m_rows.size()) - 1, 0));
}

void ProcessModel::toggleExpanded(int row)
{
    if (row < 0 || row >= m_rows.size()) {
        return;
    }
    const int pid = m_rows.at(row).pid;
    if (m_nodes.value(pid).children.isEmpty()) {
        return;
    }
    if (m_expanded.contains(pid)) {
        m_expanded.remove(pid);
    } else {
        m_expanded.insert(pid);
    }
    updateFlat();
}

void ProcessModel::expandAll()
{
    for (auto it = m_nodes.constBegin(); it != m_nodes.constEnd(); ++it) {
        if (!it->children.isEmpty()) {
            m_expanded.insert(it.key());
        }
    }
    updateFlat();
}

void ProcessModel::collapseAll()
{
    m_expanded.clear();
    updateFlat();
}

int ProcessModel::focusPid(int pid)
{
    const auto &procs = m_sampler->processes();
    const auto it = procs.constFind(pid);
    if (it == procs.constEnd()) {
        return -1;
    }

    if (!matchesFilter(it.value())) {
        m_filterMode = 3;
        emit filterModeChanged();
        m_expanded.clear();
        rebuild();
    }

    int parent = m_nodes.value(pid).parent;
    int guard = 0;
    while (parent > 0 && m_nodes.contains(parent) && guard++ < 4096) {
        m_expanded.insert(parent);
        parent = m_nodes.value(parent).parent;
    }
    updateFlat();

    for (int i = 0; i < m_rows.size(); ++i) {
        if (m_rows.at(i).pid == pid) {
            return i;
        }
    }
    return -1;
}

bool ProcessModel::hasProcess(int pid) const
{
    return m_sampler->processes().contains(pid);
}
