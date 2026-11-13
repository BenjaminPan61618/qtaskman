#pragma once

#include <QHash>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QVariantList>
#include <QVector>

class SystemMonitor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double cpuPercent READ cpuPercent NOTIFY statsChanged)
    Q_PROPERTY(QVariantList perCore READ perCore NOTIFY statsChanged)
    Q_PROPERTY(double memUsed READ memUsed NOTIFY statsChanged)
    Q_PROPERTY(double memTotal READ memTotal NOTIFY statsChanged)
    Q_PROPERTY(double memPercent READ memPercent NOTIFY statsChanged)
    Q_PROPERTY(double swapUsed READ swapUsed NOTIFY statsChanged)
    Q_PROPERTY(double swapTotal READ swapTotal NOTIFY statsChanged)
    Q_PROPERTY(double netUp READ netUp NOTIFY statsChanged)
    Q_PROPERTY(double netDown READ netDown NOTIFY statsChanged)
    Q_PROPERTY(QVariantList interfaces READ interfaces NOTIFY statsChanged)
    Q_PROPERTY(double diskRead READ diskRead NOTIFY statsChanged)
    Q_PROPERTY(double diskWrite READ diskWrite NOTIFY statsChanged)
    Q_PROPERTY(QVariantList mounts READ mounts NOTIFY statsChanged)
    Q_PROPERTY(double cpuTemp READ cpuTemp NOTIFY statsChanged)
    Q_PROPERTY(double memTemp READ memTemp NOTIFY statsChanged)
    Q_PROPERTY(QVariantList sensors READ sensors NOTIFY statsChanged)
    Q_PROPERTY(double batteryPercent READ batteryPercent NOTIFY statsChanged)
    Q_PROPERTY(double batteryRate READ batteryRate NOTIFY statsChanged)
    Q_PROPERTY(QString batteryState READ batteryState NOTIFY statsChanged)
    Q_PROPERTY(QVariantList cpuHistory READ cpuHistory NOTIFY statsChanged)
    Q_PROPERTY(QVariantList memHistory READ memHistory NOTIFY statsChanged)
    Q_PROPERTY(QVariantList netUpHistory READ netUpHistory NOTIFY statsChanged)
    Q_PROPERTY(QVariantList netDownHistory READ netDownHistory NOTIFY statsChanged)
    Q_PROPERTY(QVariantList diskReadHistory READ diskReadHistory NOTIFY statsChanged)
    Q_PROPERTY(QVariantList diskWriteHistory READ diskWriteHistory NOTIFY statsChanged)
    Q_PROPERTY(QVariantList cpuTempHistory READ cpuTempHistory NOTIFY statsChanged)
    Q_PROPERTY(QVariantList memTempHistory READ memTempHistory NOTIFY statsChanged)
    Q_PROPERTY(qint64 uptimeSeconds READ uptimeSeconds NOTIFY statsChanged)

public:
    explicit SystemMonitor(QObject *parent = nullptr);

    double cpuPercent() const { return m_cpuPercent; }
    QVariantList perCore() const { return m_perCore; }
    double memUsed() const { return m_memUsed; }
    double memTotal() const { return m_memTotal; }
    double memPercent() const { return m_memPercent; }
    double swapUsed() const { return m_swapUsed; }
    double swapTotal() const { return m_swapTotal; }
    double netUp() const { return m_netUp; }
    double netDown() const { return m_netDown; }
    QVariantList interfaces() const { return m_interfaces; }
    double diskRead() const { return m_diskRead; }
    double diskWrite() const { return m_diskWrite; }
    QVariantList mounts() const { return m_mounts; }
    double cpuTemp() const { return m_cpuTemp; }
    double memTemp() const { return m_memTemp; }
    QVariantList sensors() const { return m_sensors; }
    double batteryPercent() const { return m_batteryPercent; }
    double batteryRate() const { return m_batteryRate; }
    QString batteryState() const { return m_batteryState; }
    QVariantList cpuHistory() const { return m_cpuHistory; }
    QVariantList memHistory() const { return m_memHistory; }
    QVariantList netUpHistory() const { return m_netUpHistory; }
    QVariantList netDownHistory() const { return m_netDownHistory; }
    QVariantList diskReadHistory() const { return m_diskReadHistory; }
    QVariantList diskWriteHistory() const { return m_diskWriteHistory; }
    QVariantList cpuTempHistory() const { return m_cpuTempHistory; }
    QVariantList memTempHistory() const { return m_memTempHistory; }
    qint64 uptimeSeconds() const { return m_uptimeSeconds; }

    Q_INVOKABLE QString formatBytes(double bytes) const;
    Q_INVOKABLE QString formatRate(double bytesPerSecond) const;
    Q_INVOKABLE QString formatPercent(double value, int decimals = 1) const;
    Q_INVOKABLE QString formatUptime(qint64 seconds) const;

    Q_INVOKABLE void setSampleInterval(int ms);
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void clearHistory();
    Q_INVOKABLE QString exportReport();

signals:
    void statsChanged();

private:
    void sample();
    void sampleCpu();
    void sampleMemory();
    void sampleNetwork(double seconds);
    void sampleDisk(double seconds);
    void sampleMounts();
    void sampleTemperatures();
    void sampleBattery();
    void sampleUptime();
    static void pushHistory(QVariantList &history, double value);

    QTimer m_timer;
    qint64 m_lastSampleMs = 0;

    QVector<qint64> m_prevCpuBusy;
    QVector<qint64> m_prevCpuTotal;
    QHash<QString, qint64> m_prevNetRx;
    QHash<QString, qint64> m_prevNetTx;
    QHash<QString, qint64> m_prevDiskRead;
    QHash<QString, qint64> m_prevDiskWrite;

    double m_cpuPercent = 0.0;
    QVariantList m_perCore;
    double m_memUsed = 0.0;
    double m_memTotal = 0.0;
    double m_memPercent = 0.0;
    double m_swapUsed = 0.0;
    double m_swapTotal = 0.0;
    double m_netUp = 0.0;
    double m_netDown = 0.0;
    QVariantList m_interfaces;
    double m_diskRead = 0.0;
    double m_diskWrite = 0.0;
    QVariantList m_mounts;
    double m_cpuTemp = -1.0;
    double m_memTemp = -1.0;
    QVariantList m_sensors;
    double m_batteryPercent = -1.0;
    double m_batteryRate = -1.0;
    QString m_batteryState;

    QVariantList m_cpuHistory;
    QVariantList m_memHistory;
    QVariantList m_netUpHistory;
    QVariantList m_netDownHistory;
    QVariantList m_diskReadHistory;
    QVariantList m_diskWriteHistory;
    QVariantList m_cpuTempHistory;
    QVariantList m_memTempHistory;
    qint64 m_uptimeSeconds = 0;
};
