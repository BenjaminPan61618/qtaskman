#pragma once

#include <QObject>
#include <QPalette>

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    explicit ThemeManager(QObject *parent = nullptr);

    // mode: "system" | "light" | "dark"
    Q_INVOKABLE void apply(const QString &mode);

private:
    QPalette m_systemPalette;
    bool m_forced = false;
};
