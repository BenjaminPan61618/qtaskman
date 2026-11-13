#pragma once

#include <QObject>
#include <QString>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString themeMode READ themeMode WRITE setThemeMode NOTIFY themeModeChanged)
    Q_PROPERTY(QString tempUnit READ tempUnit WRITE setTempUnit NOTIFY tempUnitChanged)
    Q_PROPERTY(int sampleInterval READ sampleInterval WRITE setSampleInterval NOTIFY sampleIntervalChanged)
    Q_PROPERTY(bool stayInTray READ stayInTray WRITE setStayInTray NOTIFY stayInTrayChanged)
    Q_PROPERTY(double floatOpacity READ floatOpacity WRITE setFloatOpacity NOTIFY floatOpacityChanged)

public:
    explicit Settings(QObject *parent = nullptr);

    QString themeMode() const { return m_themeMode; }
    QString tempUnit() const { return m_tempUnit; }
    int sampleInterval() const { return m_sampleInterval; }
    bool stayInTray() const { return m_stayInTray; }
    double floatOpacity() const { return m_floatOpacity; }

    Q_INVOKABLE void setThemeMode(const QString &mode);
    Q_INVOKABLE void setTempUnit(const QString &unit);
    Q_INVOKABLE void setSampleInterval(int ms);
    Q_INVOKABLE void setStayInTray(bool stay);
    Q_INVOKABLE void setFloatOpacity(double opacity);

    Q_INVOKABLE double toDisplayTemp(double celsius) const;
    Q_INVOKABLE QString tempUnitSymbol() const;

signals:
    void themeModeChanged();
    void tempUnitChanged();
    void sampleIntervalChanged();
    void stayInTrayChanged();
    void floatOpacityChanged();

private:
    void load();
    void save();

    QString m_themeMode = QStringLiteral("system");
    QString m_tempUnit = QStringLiteral("C");
    int m_sampleInterval = 1000;
    bool m_stayInTray = false;
    double m_floatOpacity = 0.85;
};
