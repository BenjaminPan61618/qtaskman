#pragma once

#include <QHash>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QVector>

#include <sys/types.h>

struct ProcMetrics
{
    int pid = 0;
    int ppid = 0;
    uid_t uid = 0;
    int nice = 0;
    QString name;
    QString exe;
    QString cmdline;
    QString user;
    QString iconName;

    double cpuPercent = 0.0;
    qint64 rssBytes = 0;
    double memPercent = 0.0;

    double netUp = 0.0;
    double netDown = 0.0;

    double diskRead = 0.0;
    double diskWrite = 0.0;

    // Process state from /proc/<pid>/stat (R, S, D, Z, T, t, ...)
    QChar state;
    // Seconds since the process started; -1 when unknown.
    qint64 ageSeconds = -1;

    // internal sampling bookkeeping
    qint64 cpuTicks = 0;
    qint64 diskReadTotal = 0;
    qint64 diskWriteTotal = 0;
};

class ProcessSampler : public QObject
{
    Q_OBJECT

public:
    explicit ProcessSampler(QObject *parent = nullptr);

    void start(int intervalMs = 1000);
    void stop();
    void sampleNow();

    const QHash<int, ProcMetrics> &processes() const { return m_processes; }
    qint64 totalMemory() const { return m_totalMemory; }
    uid_t selfUid() const { return m_selfUid; }
    int selfPid() const;

    QString desktopNameForExe(const QString &exe) const;

signals:
    void sampled();

private:
    void sample();
    void sampleNetwork(double seconds);
    void loadDesktopEntries();
    QString resolveIcon(const QString &exe);

    QTimer m_timer;
    QHash<int, ProcMetrics> m_processes;
    QHash<int, qint64> m_prevCpuTicks;
    QHash<int, qint64> m_prevDiskRead;
    QHash<int, qint64> m_prevDiskWrite;
    QHash<int, qint64> m_prevNetSent;
    QHash<int, qint64> m_prevNetRecv;

    qint64 m_lastSampleMs = 0;
    qint64 m_lastNetMs = 0;
    qint64 m_totalMemory = 0;
    double m_uptimeSeconds = 0.0;
    uid_t m_selfUid = 0;
    int m_ticksPerSecond = 100;

    QHash<QString, QString> m_desktopIcons;
    QHash<QString, QString> m_desktopNames;
    QHash<QString, QString> m_iconCache;
    bool m_desktopLoaded = false;
};
