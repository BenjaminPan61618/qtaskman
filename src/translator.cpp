#include "translator.h"

#include <QCoreApplication>
#include <QDir>
#include <QLocale>
#include <QSettings>
#include <QQmlEngine>

namespace {
constexpr auto SettingsGroup = "General";
constexpr auto SettingsLanguageKey = "language";
constexpr auto ResourcePrefix = ":/i18n/qtaskman";
} // namespace

Translator::Translator(QQmlEngine *engine, QObject *parent)
    : QObject(parent)
    , m_engine(engine)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       QStringLiteral("qtaskman"), QStringLiteral("qtaskman"));
    settings.beginGroup(QString::fromLatin1(SettingsGroup));
    const QString saved = settings.value(QString::fromLatin1(SettingsLanguageKey)).toString();
    settings.endGroup();

    const QString envOverride = qEnvironmentVariable("QTASKMAN_LANGUAGE");
    const QString code = !envOverride.isEmpty() ? envOverride
                                                 : (saved.isEmpty() ? QStringLiteral("system") : saved);
    loadTranslation(code);
    m_language = code;
}

QVariantList Translator::languages() const
{
    return {
        QVariantMap{{QStringLiteral("code"), QStringLiteral("system")},
                    {QStringLiteral("name"), tr("跟随系统")},
                    {QStringLiteral("native"), QStringLiteral("System")}},
        QVariantMap{{QStringLiteral("code"), QStringLiteral("zh_CN")},
                    {QStringLiteral("name"), tr("简体中文")},
                    {QStringLiteral("native"), QStringLiteral("简体中文")}},
        QVariantMap{{QStringLiteral("code"), QStringLiteral("en_US")},
                    {QStringLiteral("name"), tr("英语")},
                    {QStringLiteral("native"), QStringLiteral("English")}},
    };
}

bool Translator::loadTranslation(const QString &code)
{
    QString localeCode = code;
    if (localeCode == QLatin1String("system")) {
        localeCode = QLocale::system().name();
    }
    // Try the exact locale first (e.g. zh_CN), then fall back to the language (zh).
    QStringList candidates{localeCode};
    const int underscore = localeCode.indexOf(QLatin1Char('_'));
    if (underscore > 0) {
        candidates << localeCode.left(underscore);
    }

    for (const QString &candidate : candidates) {
        const QString path = QStringLiteral("%1_%2.qm")
                                 .arg(QString::fromLatin1(ResourcePrefix))
                                 .arg(candidate);
        const bool ok = m_translator.load(path);
        if (ok) {
            qApp->installTranslator(&m_translator);
            return true;
        }
    }
    qApp->removeTranslator(&m_translator);
    return false;
}

void Translator::setLanguage(const QString &code)
{
    if (code == m_language) {
        return;
    }
    loadTranslation(code);
    m_language = code;

    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       QStringLiteral("qtaskman"), QStringLiteral("qtaskman"));
    settings.beginGroup(QString::fromLatin1(SettingsGroup));
    settings.setValue(QString::fromLatin1(SettingsLanguageKey), code);
    settings.endGroup();

    emit languageChanged();
    if (m_engine) {
        m_engine->retranslate();
    }
}
