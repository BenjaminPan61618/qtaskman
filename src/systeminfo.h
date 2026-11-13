#pragma once

#include <QObject>
#include <QString>

class SystemInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString distro READ distro CONSTANT)
    Q_PROPERTY(QString kernel READ kernel CONSTANT)
    Q_PROPERTY(QString arch READ arch CONSTANT)
    Q_PROPERTY(QString bits READ bits CONSTANT)
    Q_PROPERTY(QString memory READ memory CONSTANT)
    Q_PROPERTY(QString gpu READ gpu CONSTANT)
    Q_PROPERTY(QString sessionType READ sessionType CONSTANT)
    Q_PROPERTY(QString desktop READ desktop CONSTANT)
    Q_PROPERTY(QString hostname READ hostname CONSTANT)
    Q_PROPERTY(QString cpuModel READ cpuModel CONSTANT)
    Q_PROPERTY(int cpuCores READ cpuCores CONSTANT)

public:
    explicit SystemInfo(QObject *parent = nullptr);

    QString distro() const { return m_distro; }
    QString kernel() const { return m_kernel; }
    QString arch() const { return m_arch; }
    QString bits() const { return m_bits; }
    QString memory() const { return m_memory; }
    QString gpu() const { return m_gpu; }
    QString sessionType() const { return m_sessionType; }
    QString desktop() const { return m_desktop; }
    QString hostname() const { return m_hostname; }
    QString cpuModel() const { return m_cpuModel; }
    int cpuCores() const { return m_cpuCores; }

private:
    static QString osReleaseValue(const QString &key);
    static QString detectGpu();
    static QString detectDesktop();
    static QString detectSession();
    static QString readCpuModel();
    static QString humanBytes(qint64 bytes);

    QString m_distro;
    QString m_kernel;
    QString m_arch;
    QString m_bits;
    QString m_memory;
    QString m_gpu;
    QString m_sessionType;
    QString m_desktop;
    QString m_hostname;
    QString m_cpuModel;
    int m_cpuCores = 0;
};
