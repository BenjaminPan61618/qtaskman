#include <QApplication>
#include <QDir>
#include <QIcon>
#include <QMenu>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QSystemTrayIcon>
#include <QTimer>

#include "appsmodel.h"
#include "processcontroller.h"
#include "processmodel.h"
#include "processsampler.h"
#include "settings.h"
#include "systeminfo.h"
#include "systemmonitor.h"
#include "thememanager.h"
#include "translator.h"
#include "windowprobe.h"

int main(int argc, char *argv[])
{
    // Kirigami QML modules usually live in the distribution's Qt prefix, which
    // can differ from the prefix of the Qt used to build this application.
    QStringList importPaths = qEnvironmentVariable("QML_IMPORT_PATH").split(QLatin1Char(':'), Qt::SkipEmptyParts);
    const QStringList distroImportPaths{
        QStringLiteral("/usr/lib64/qt6/qml"),
        QStringLiteral("/usr/lib/qt6/qml"),
        QStringLiteral("/usr/lib/x86_64-linux-gnu/qt6/qml"),
    };
    for (const QString &path : distroImportPaths) {
        if (QDir(path).exists() && !importPaths.contains(path)) {
            importPaths.append(path);
        }
    }
    if (!importPaths.isEmpty()) {
        qputenv("QML_IMPORT_PATH", importPaths.join(QLatin1Char(':')).toUtf8());
    }

    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("qtaskman"));
    app.setApplicationDisplayName(QStringLiteral("qTaskMan"));
    app.setApplicationVersion(QStringLiteral("1.0.0"));
    app.setOrganizationName(QStringLiteral("qtaskman"));
    app.setDesktopFileName(QStringLiteral("org.qtaskman.qtaskman"));

    QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));

    // The Qt installed by aqt does not inherit the system's XDG icon search
    // paths, so add them explicitly before selecting the Breeze theme.
    QStringList iconPaths = QIcon::themeSearchPaths();
    const QStringList distroIconPaths{
        QStringLiteral("/usr/share/icons"),
        QStringLiteral("/usr/local/share/icons"),
        QDir::homePath() + QStringLiteral("/.local/share/icons"),
        QDir::homePath() + QStringLiteral("/.icons"),
    };
    for (const QString &path : distroIconPaths) {
        if (QDir(path).exists() && !iconPaths.contains(path)) {
            iconPaths.append(path);
        }
    }
    QIcon::setThemeSearchPaths(iconPaths);
    QIcon::setThemeName(QStringLiteral("breeze"));

    ProcessSampler sampler;
    SystemMonitor monitor;
    SystemInfo systemInfo;
    ProcessModel processModel(&sampler);
    WindowProbe windowProbe;
    AppsModel appsModel(&sampler, &windowProbe);
    ProcessController processController;
    Settings settings;
    ThemeManager themeManager;

    themeManager.apply(settings.themeMode());
    QObject::connect(&settings, &Settings::themeModeChanged, &themeManager,
                     [&themeManager, &settings]() { themeManager.apply(settings.themeMode()); });

    monitor.setSampleInterval(settings.sampleInterval());
    QObject::connect(&settings, &Settings::sampleIntervalChanged, &monitor,
                     [&monitor, &settings]() { monitor.setSampleInterval(settings.sampleInterval()); });

    QQmlApplicationEngine engine;
    engine.addImportPath(QStringLiteral("qrc:/qml"));
    for (const QString &path : importPaths) {
        engine.addImportPath(path);
    }

    Translator translator(&engine);

    qmlRegisterSingletonInstance("qtaskman", 1, 0, "Translator", &translator);

    engine.rootContext()->setContextProperty(QStringLiteral("systemInfo"), &systemInfo);
    engine.rootContext()->setContextProperty(QStringLiteral("monitor"), &monitor);
    engine.rootContext()->setContextProperty(QStringLiteral("processModel"), &processModel);
    engine.rootContext()->setContextProperty(QStringLiteral("appModel"), &appsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("procController"), &processController);
    engine.rootContext()->setContextProperty(QStringLiteral("appSettings"), &settings);
    engine.rootContext()->setContextProperty(QStringLiteral("trayAvailable"),
                                             QSystemTrayIcon::isSystemTrayAvailable());

    windowProbe.start();
    sampler.start();

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return 1;
    }

    // ----------------------------------------------------------------- Tray
    auto *window = qobject_cast<QQuickWindow *>(engine.rootObjects().constFirst());

    QIcon trayIcon(QStringLiteral(":/assets/qtaskman.svg"));
    if (trayIcon.isNull()) {
        trayIcon = QIcon::fromTheme(QStringLiteral("utilities-system-monitor"));
    }
    if (trayIcon.isNull()) {
        trayIcon = QIcon::fromTheme(QStringLiteral("system-monitoring"));
    }

    auto *tray = new QSystemTrayIcon(trayIcon, &app);
    tray->setToolTip(QStringLiteral("qTaskMan"));

    auto *trayMenu = new QMenu();
    auto *toggleAction = trayMenu->addAction(QString());
    auto *quitAction = trayMenu->addAction(QString());
    tray->setContextMenu(trayMenu);

    auto retranslateTray = [trayMenu, toggleAction, quitAction, window]() {
        toggleAction->setText(window->isVisible()
                                  ? QObject::tr("隐藏窗口")
                                  : QObject::tr("显示窗口"));
        quitAction->setText(QObject::tr("退出"));
    };
    retranslateTray();

    QObject::connect(toggleAction, &QAction::triggered, window, [window, retranslateTray]() {
        window->setVisible(!window->isVisible());
        retranslateTray();
    });
    QObject::connect(quitAction, &QAction::triggered, &app, &QCoreApplication::quit);

    QObject::connect(tray, &QSystemTrayIcon::activated, window,
                     [window, retranslateTray](QSystemTrayIcon::ActivationReason reason) {
                         if (reason == QSystemTrayIcon::Trigger
                             || reason == QSystemTrayIcon::DoubleClick) {
                             window->setVisible(!window->isVisible());
                             if (window->isVisible()) {
                                 window->raise();
                                 window->requestActivate();
                             }
                             retranslateTray();
                         }
                     });

    QObject::connect(window, &QWindow::visibleChanged, retranslateTray);
    QObject::connect(&translator, &Translator::languageChanged, retranslateTray);

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        tray->show();
    }

    // Development helper: renders the window off-screen for visual verification.
    const QString screenshotPath = qEnvironmentVariable("QTASKMAN_SCREENSHOT");
    if (!screenshotPath.isEmpty()) {
        QTimer::singleShot(2500, &app, [&engine, screenshotPath]() {
            auto *window = qobject_cast<QQuickWindow *>(engine.rootObjects().constFirst());
            if (!window) {
                QCoreApplication::quit();
                return;
            }
            const QString section = qEnvironmentVariable("QTASKMAN_SECTION");
            if (!section.isEmpty()) {
                window->setProperty("section", section.toInt());
            }
            const QString tab = qEnvironmentVariable("QTASKMAN_MONITOR_TAB");
            if (!tab.isEmpty()) {
                window->setProperty("monitorTab", tab.toInt());
            }
            QTimer::singleShot(400, window, [window, screenshotPath]() {
                window->grabWindow().save(screenshotPath);
                QCoreApplication::quit();
            });
        });
    }

    return app.exec();
}
