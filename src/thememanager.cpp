#include "thememanager.h"

#include <QApplication>
#include <QDir>
#include <QIcon>

namespace {

QPalette lightPalette()
{
    QPalette p;
    p.setColor(QPalette::Window, QColor(QStringLiteral("#efefef")));
    p.setColor(QPalette::WindowText, QColor(QStringLiteral("#000000")));
    p.setColor(QPalette::Base, QColor(QStringLiteral("#ffffff")));
    p.setColor(QPalette::AlternateBase, QColor(QStringLiteral("#f7f7f7")));
    p.setColor(QPalette::Text, QColor(QStringLiteral("#000000")));
    p.setColor(QPalette::Button, QColor(QStringLiteral("#efefef")));
    p.setColor(QPalette::ButtonText, QColor(QStringLiteral("#000000")));
    p.setColor(QPalette::BrightText, QColor(QStringLiteral("#ffffff")));
    p.setColor(QPalette::Highlight, QColor(QStringLiteral("#308cc6")));
    p.setColor(QPalette::HighlightedText, QColor(QStringLiteral("#ffffff")));
    p.setColor(QPalette::PlaceholderText, QColor(0, 0, 0, 128));
    p.setColor(QPalette::ToolTipBase, QColor(QStringLiteral("#ffffdc")));
    p.setColor(QPalette::ToolTipText, QColor(QStringLiteral("#000000")));
    p.setColor(QPalette::Mid, QColor(QStringLiteral("#b8b8b8")));
    p.setColor(QPalette::Dark, QColor(QStringLiteral("#9f9f9f")));
    p.setColor(QPalette::Light, QColor(QStringLiteral("#ffffff")));
    p.setColor(QPalette::Link, QColor(QStringLiteral("#0000ff")));
    p.setColor(QPalette::LinkVisited, QColor(QStringLiteral("#ff00ff")));
    const QColor disabled(QStringLiteral("#bebebe"));
    p.setColor(QPalette::Disabled, QPalette::WindowText, disabled);
    p.setColor(QPalette::Disabled, QPalette::Text, disabled);
    p.setColor(QPalette::Disabled, QPalette::ButtonText, disabled);
    return p;
}

QPalette darkPalette()
{
    QPalette p;
    p.setColor(QPalette::Window, QColor(QStringLiteral("#31363b")));
    p.setColor(QPalette::WindowText, QColor(QStringLiteral("#eff0f1")));
    p.setColor(QPalette::Base, QColor(QStringLiteral("#232629")));
    p.setColor(QPalette::AlternateBase, QColor(QStringLiteral("#2a2e32")));
    p.setColor(QPalette::Text, QColor(QStringLiteral("#eff0f1")));
    p.setColor(QPalette::Button, QColor(QStringLiteral("#31363b")));
    p.setColor(QPalette::ButtonText, QColor(QStringLiteral("#eff0f1")));
    p.setColor(QPalette::BrightText, QColor(QStringLiteral("#ffffff")));
    p.setColor(QPalette::Highlight, QColor(QStringLiteral("#3daee9")));
    p.setColor(QPalette::HighlightedText, QColor(QStringLiteral("#ffffff")));
    p.setColor(QPalette::PlaceholderText, QColor(QStringLiteral("#7f8c8d")));
    p.setColor(QPalette::ToolTipBase, QColor(QStringLiteral("#232629")));
    p.setColor(QPalette::ToolTipText, QColor(QStringLiteral("#eff0f1")));
    p.setColor(QPalette::Mid, QColor(QStringLiteral("#565b60")));
    p.setColor(QPalette::Dark, QColor(QStringLiteral("#1f2224")));
    p.setColor(QPalette::Light, QColor(QStringLiteral("#3a4046")));
    p.setColor(QPalette::Link, QColor(QStringLiteral("#2980b9")));
    p.setColor(QPalette::LinkVisited, QColor(QStringLiteral("#9b59b6")));
    const QColor disabled(QStringLiteral("#7f8c8d"));
    p.setColor(QPalette::Disabled, QPalette::WindowText, disabled);
    p.setColor(QPalette::Disabled, QPalette::Text, disabled);
    p.setColor(QPalette::Disabled, QPalette::ButtonText, disabled);
    return p;
}

bool iconThemeExists(const QString &name)
{
    for (const QString &path : QIcon::themeSearchPaths()) {
        if (QDir(path + QLatin1Char('/') + name).exists()) {
            return true;
        }
    }
    return false;
}

// Breeze ships a light and a dark icon variant; symbolic icons are not
// recoloured automatically, so the matching variant has to be selected.
void applyIconTheme(bool dark)
{
    QString base = QIcon::themeName();
    if (base.endsWith(QStringLiteral("-dark"))) {
        base.chop(5);
    }
    if (base.isEmpty()) {
        base = QStringLiteral("breeze");
    }
    const QString wanted = dark ? base + QStringLiteral("-dark") : base;
    if (QIcon::themeName() != wanted && iconThemeExists(wanted)) {
        QIcon::setThemeName(wanted);
    }
}

} // namespace

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
    , m_systemPalette(QGuiApplication::palette())
{
}

void ThemeManager::apply(const QString &mode)
{
    bool dark = false;
    if (mode == QLatin1String("dark")) {
        QApplication::setPalette(darkPalette());
        m_forced = true;
        dark = true;
    } else if (mode == QLatin1String("light")) {
        QApplication::setPalette(lightPalette());
        m_forced = true;
    } else {
        if (m_forced) {
            // Back to the platform palette captured before any override.
            QApplication::setPalette(m_systemPalette);
            m_forced = false;
        }
        dark = m_systemPalette.color(QPalette::Window).lightness() < 128;
    }
    applyIconTheme(dark);
}
