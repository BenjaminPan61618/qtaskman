#pragma once

#include <QAbstractListModel>
#include <QHash>
#include <QSet>
#include <QVector>

class ProcessSampler;
class WindowProbe;

class AppsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        PidRole = Qt::UserRole + 1,
        AppNameRole,
        TitleRole,
        IconNameRole,
        CpuRole,
        MemRole,
        MemPercentRole,
        NetUpRole,
        NetDownRole,
        DiskReadRole,
        DiskWriteRole,
        ActiveWindowRole,
        PowerRole,
        StateRole,
        IsNewRole,
    };
    Q_ENUM(Roles)

    AppsModel(ProcessSampler *sampler, WindowProbe *probe, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void countChanged();

private slots:
    void rebuild();
    void refreshValues();
    void onSampled();

private:
    struct AppRow {
        int pid = 0;
        QString name;
        QString title;
        QString icon;
        bool active = false;
    };

    bool looksLikeGuiApp(int pid);
    QVector<AppRow> buildFromWindows() const;
    QVector<AppRow> buildFallback();

    ProcessSampler *m_sampler = nullptr;
    WindowProbe *m_probe = nullptr;
    QVector<AppRow> m_rows;
    QHash<int, bool> m_guiCache;
    QSet<int> m_fallbackPids;
};
