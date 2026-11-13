#include "windowprobe.h"

#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

namespace {
constexpr auto ServiceName = "org.qtaskman.Windows";
constexpr auto ObjectPath = "/Windows";

const char *kProbeScript = R"JS(
var wins = workspace.windowList();
var out = [];
for (var i = 0; i < wins.length; ++i) {
    var w = wins[i];
    if (!w || w.deleted || w.skipTaskbar || w.skipSwitcher) {
        continue;
    }
    out.push({
        pid: w.pid,
        caption: w.caption || "",
        cls: w.resourceClass || "",
        name: w.resourceName || "",
        active: w.active === true
    });
}
callDBus("org.qtaskman.Windows", "/Windows", "org.qtaskman.Windows", "reportWindows", JSON.stringify(out));
)JS";
} // namespace

WindowProbe::WindowProbe(QObject *parent)
    : QObject(parent)
{
    m_timer.setInterval(2000);
    connect(&m_timer, &QTimer::timeout, this, &WindowProbe::requestWindows);
}

WindowProbe::~WindowProbe()
{
    if (m_serviceRegistered) {
        QDBusConnection::sessionBus().unregisterObject(QString::fromLatin1(ObjectPath));
        QDBusConnection::sessionBus().unregisterService(QString::fromLatin1(ServiceName));
    }
    unloadScript();
    if (!m_scriptPath.isEmpty()) {
        QFile::remove(m_scriptPath);
    }
}

void WindowProbe::start()
{
    if (m_started) {
        return;
    }
    m_started = true;

    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.isConnected() || !bus.interface()->isServiceRegistered(QStringLiteral("org.kde.KWin"))) {
        return;
    }

    m_serviceRegistered = bus.registerService(QString::fromLatin1(ServiceName))
        && bus.registerObject(QString::fromLatin1(ObjectPath), this,
                              QDBusConnection::ExportScriptableSlots);
    if (!m_serviceRegistered) {
        return;
    }

    m_scriptPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation)
        + QStringLiteral("/qtaskman-windows-%1.js").arg(QCoreApplication::applicationPid());
    QFile script(m_scriptPath);
    if (!script.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return;
    }
    script.write(kProbeScript);
    script.close();

    loadScript();
    if (m_scriptId < 0) {
        return;
    }
    runScript();

    m_lastReport.start();
    m_timer.start();
}

void WindowProbe::loadScript()
{
    QDBusMessage message = QDBusMessage::createMethodCall(
        QStringLiteral("org.kde.KWin"), QStringLiteral("/Scripting"),
        QStringLiteral("org.kde.kwin.Scripting"), QStringLiteral("loadScript"));
    message.setArguments({m_scriptPath, QStringLiteral("qtaskman-windows")});
    const QDBusMessage reply = QDBusConnection::sessionBus().call(message);
    if (reply.type() != QDBusMessage::ReplyMessage || reply.arguments().isEmpty()) {
        return;
    }
    m_scriptId = reply.arguments().constFirst().toInt();
}

void WindowProbe::unloadScript()
{
    QDBusMessage message = QDBusMessage::createMethodCall(
        QStringLiteral("org.kde.KWin"), QStringLiteral("/Scripting"),
        QStringLiteral("org.kde.kwin.Scripting"), QStringLiteral("unloadScript"));
    message.setArguments({QStringLiteral("qtaskman-windows")});
    QDBusConnection::sessionBus().call(message);
    m_scriptId = -1;
}

void WindowProbe::runScript()
{
    if (m_scriptId < 0) {
        return;
    }
    QDBusInterface script(QStringLiteral("org.kde.KWin"),
                          QStringLiteral("/Scripting/Script%1").arg(m_scriptId),
                          QStringLiteral("org.kde.kwin.Script"), QDBusConnection::sessionBus());
    if (script.isValid()) {
        script.asyncCall(QStringLiteral("run"));
    }
}

void WindowProbe::requestWindows()
{
    // KWin only executes a loaded script once, so reload a fresh instance for
    // every poll to keep receiving updates.
    unloadScript();
    loadScript();
    runScript();

    if (m_lastReport.isValid() && m_lastReport.elapsed() > 6000 && m_available) {
        setAvailable(false);
    }
}

void WindowProbe::setAvailable(bool available)
{
    if (m_available == available) {
        return;
    }
    m_available = available;
    emit availableChanged();
}

void WindowProbe::reportWindows(const QString &json)
{
    const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isArray()) {
        return;
    }

    QVector<Window> next;
    const QJsonArray array = doc.array();
    next.reserve(array.size());
    for (const auto &value : array) {
        const QJsonObject object = value.toObject();
        Window window;
        window.pid = object.value(QStringLiteral("pid")).toInt();
        window.caption = object.value(QStringLiteral("caption")).toString().trimmed();
        window.cls = object.value(QStringLiteral("cls")).toString();
        window.name = object.value(QStringLiteral("name")).toString();
        window.active = object.value(QStringLiteral("active")).toBool();

        if (window.pid <= 0 || window.caption.isEmpty()) {
            continue;
        }
        if (window.cls.startsWith(QLatin1String("kwin"))) {
            continue;
        }
        next.append(window);
    }

    m_lastReport.restart();
    if (!m_available) {
        setAvailable(true);
    }

    bool changed = next.size() != m_windows.size();
    if (!changed) {
        for (int i = 0; i < next.size(); ++i) {
            const Window &a = next.at(i);
            const Window &b = m_windows.at(i);
            if (a.pid != b.pid || a.caption != b.caption || a.cls != b.cls || a.active != b.active) {
                changed = true;
                break;
            }
        }
    }
    m_windows = next;
    if (changed) {
        emit windowsChanged();
    }
}
