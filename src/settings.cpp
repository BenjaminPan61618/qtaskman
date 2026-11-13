#include "settings.h"

#include <QSettings>

namespace {
constexpr auto SettingsGroup = "General";
constexpr auto ThemeModeKey = "themeMode";
constexpr auto TempUnitKey = "tempUnit";
constexpr auto SampleIntervalKey = "sampleInterval";
constexpr auto StayInTrayKey = "stayInTray";
constexpr auto FloatOpacityKey = "floatOpacity";
constexpr double FloatOpacityMin = 0.20;
constexpr double FloatOpacityMax = 1.00;
} // namespace

Settings::Settings(QObject *parent)
    : QObject(parent)
{
    load();
}

void Settings::load()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       QStringLiteral("qtaskman"), QStringLiteral("qtaskman"));
    settings.beginGroup(QString::fromLatin1(SettingsGroup));
    m_themeMode = settings.value(QString::fromLatin1(ThemeModeKey), QStringLiteral("system")).toString();
    if (m_themeMode != QLatin1String("light") && m_themeMode != QLatin1String("dark")) {
        m_themeMode = QStringLiteral("system");
    }
    m_tempUnit = settings.value(QString::fromLatin1(TempUnitKey), QStringLiteral("C")).toString();
    m_sampleInterval = settings.value(QString::fromLatin1(SampleIntervalKey), 1000).toInt();
    m_stayInTray = settings.value(QString::fromLatin1(StayInTrayKey), false).toBool();
    m_floatOpacity = settings.value(QString::fromLatin1(FloatOpacityKey), 0.85).toDouble();
    m_floatOpacity = qBound(FloatOpacityMin, m_floatOpacity, FloatOpacityMax);
    if (m_sampleInterval < 500) {
        m_sampleInterval = 500;
    } else if (m_sampleInterval > 10000) {
        m_sampleInterval = 10000;
    }
    settings.endGroup();
}

void Settings::save()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       QStringLiteral("qtaskman"), QStringLiteral("qtaskman"));
    settings.beginGroup(QString::fromLatin1(SettingsGroup));
    settings.setValue(QString::fromLatin1(ThemeModeKey), m_themeMode);
    settings.setValue(QString::fromLatin1(TempUnitKey), m_tempUnit);
    settings.setValue(QString::fromLatin1(SampleIntervalKey), m_sampleInterval);
    settings.setValue(QString::fromLatin1(StayInTrayKey), m_stayInTray);
    settings.setValue(QString::fromLatin1(FloatOpacityKey), m_floatOpacity);
    settings.endGroup();
}

void Settings::setThemeMode(const QString &mode)
{
    if (mode == m_themeMode) {
        return;
    }
    if (mode != QLatin1String("system") && mode != QLatin1String("light")
        && mode != QLatin1String("dark")) {
        return;
    }
    m_themeMode = mode;
    save();
    emit themeModeChanged();
}

void Settings::setTempUnit(const QString &unit)
{
    if (unit == m_tempUnit) {
        return;
    }
    if (unit != QLatin1String("C") && unit != QLatin1String("F")) {
        return;
    }
    m_tempUnit = unit;
    save();
    emit tempUnitChanged();
}

void Settings::setSampleInterval(int ms)
{
    if (ms == m_sampleInterval) {
        return;
    }
    ms = qBound(500, ms, 10000);
    m_sampleInterval = ms;
    save();
    emit sampleIntervalChanged();
}

void Settings::setStayInTray(bool stay)
{
    if (stay == m_stayInTray) {
        return;
    }
    m_stayInTray = stay;
    save();
    emit stayInTrayChanged();
}

void Settings::setFloatOpacity(double opacity)
{
    opacity = qBound(FloatOpacityMin, opacity, FloatOpacityMax);
    if (qFuzzyCompare(opacity, m_floatOpacity)) {
        return;
    }
    m_floatOpacity = opacity;
    save();
    emit floatOpacityChanged();
}

double Settings::toDisplayTemp(double celsius) const
{
    if (celsius < 0) {
        return celsius;
    }
    if (m_tempUnit == QLatin1String("F")) {
        return celsius * 9.0 / 5.0 + 32.0;
    }
    return celsius;
}

QString Settings::tempUnitSymbol() const
{
    return m_tempUnit == QLatin1String("F") ? QStringLiteral("°F") : QStringLiteral("°C");
}
