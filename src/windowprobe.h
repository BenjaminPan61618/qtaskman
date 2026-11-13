#pragma once

#include <QElapsedTimer>
#include <QObject>
#include <QTimer>
#include <QVector>

class WindowProbe : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.qtaskman.Windows")
    Q_PROPERTY(bool available READ available NOTIFY availableChanged)

public:
    struct Window {
        int pid = 0;
        QString caption;
        QString cls;
        QString name;
        bool active = false;
    };

    explicit WindowProbe(QObject *parent = nullptr);
    ~WindowProbe() override;

    void start();
    bool available() const { return m_available; }
    const QVector<Window> &windows() const { return m_windows; }

signals:
    void windowsChanged();
    void availableChanged();

public slots:
    Q_SCRIPTABLE void reportWindows(const QString &json);

private:
    void loadScript();
    void unloadScript();
    void runScript();
    void requestWindows();
    void setAvailable(bool available);

    QTimer m_timer;
    QElapsedTimer m_lastReport;
    QVector<Window> m_windows;
    int m_scriptId = -1;
    QString m_scriptPath;
    bool m_available = false;
    bool m_started = false;
    bool m_serviceRegistered = false;
};
