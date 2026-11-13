#pragma once

#include <QObject>
#include <QString>

class ProcessController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)

public:
    explicit ProcessController(QObject *parent = nullptr);

    QString lastError() const { return m_lastError; }

    Q_INVOKABLE bool sendSignal(int pid, int signalNumber);
    Q_INVOKABLE bool setPriority(int pid, int niceValue);
    Q_INVOKABLE QString executablePath(int pid) const;
    Q_INVOKABLE bool openExecutableLocation(int pid);
    Q_INVOKABLE bool restartProcess(int pid);
    Q_INVOKABLE void clearError();

signals:
    void lastErrorChanged();

private:
    void fail(const QString &message);

    QString m_lastError;
};
