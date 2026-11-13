#pragma once

#include <QAbstractListModel>
#include <QHash>
#include <QSet>
#include <QVector>

class ProcessSampler;
struct ProcMetrics;

class ProcessModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(int filterMode READ filterMode WRITE setFilterMode NOTIFY filterModeChanged)
    Q_PROPERTY(bool flatMode READ flatMode WRITE setFlatMode NOTIFY flatModeChanged)

public:
    enum Roles {
        PidRole = Qt::UserRole + 1,
        PpidRole,
        NameRole,
        CommandRole,
        UserRole,
        CpuRole,
        MemRole,
        MemPercentRole,
        NetUpRole,
        NetDownRole,
        DiskReadRole,
        DiskWriteRole,
        IconNameRole,
        DepthRole,
        HasChildrenRole,
        ExpandedRole,
        IsSelfRole,
        NiceRole,
        StateRole,
        IsNewRole,
    };
    Q_ENUM(Roles)

    explicit ProcessModel(ProcessSampler *sampler, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int filterMode() const { return m_filterMode; }
    void setFilterMode(int mode);

    bool flatMode() const { return m_flatMode; }
    void setFlatMode(bool enabled);

    Q_INVOKABLE void toggleExpanded(int row);
    Q_INVOKABLE void expandAll();
    Q_INVOKABLE void collapseAll();
    Q_INVOKABLE int focusPid(int pid);
    Q_INVOKABLE bool hasProcess(int pid) const;

signals:
    void countChanged();
    void filterModeChanged();
    void flatModeChanged();

private slots:
    void onSampled();

private:
    struct Node {
        int pid = 0;
        int parent = 0;
        QVector<int> children;
    };
    struct Row {
        int pid = 0;
        int depth = 0;
        bool operator==(const Row &other) const { return pid == other.pid && depth == other.depth; }
    };

    bool matchesFilter(const ProcMetrics &metrics) const;
    void rebuild();
    void updateFlat();

    ProcessSampler *m_sampler = nullptr;
    QHash<int, Node> m_nodes;
    QVector<int> m_roots;
    QVector<Row> m_rows;
    QSet<int> m_expanded;
    QSet<int> m_matching;
    QSet<int> m_knownPids;
    int m_filterMode = 0;
    bool m_flatMode = false;
    bool m_firstBuild = true;
};
