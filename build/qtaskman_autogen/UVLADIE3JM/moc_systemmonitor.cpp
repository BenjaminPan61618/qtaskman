/****************************************************************************
** Meta object code from reading C++ file 'systemmonitor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/systemmonitor.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'systemmonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13SystemMonitorE_t {};
} // unnamed namespace

template <> constexpr inline auto SystemMonitor::qt_create_metaobjectdata<qt_meta_tag_ZN13SystemMonitorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SystemMonitor",
        "statsChanged",
        "",
        "formatBytes",
        "bytes",
        "formatRate",
        "bytesPerSecond",
        "formatPercent",
        "value",
        "decimals",
        "formatUptime",
        "seconds",
        "setSampleInterval",
        "ms",
        "refresh",
        "clearHistory",
        "exportReport",
        "cpuPercent",
        "perCore",
        "QVariantList",
        "memUsed",
        "memTotal",
        "memPercent",
        "swapUsed",
        "swapTotal",
        "netUp",
        "netDown",
        "interfaces",
        "diskRead",
        "diskWrite",
        "mounts",
        "cpuTemp",
        "memTemp",
        "sensors",
        "batteryPercent",
        "batteryRate",
        "batteryState",
        "cpuHistory",
        "memHistory",
        "netUpHistory",
        "netDownHistory",
        "diskReadHistory",
        "diskWriteHistory",
        "cpuTempHistory",
        "memTempHistory",
        "uptimeSeconds"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'statsChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'formatBytes'
        QtMocHelpers::MethodData<QString(double) const>(3, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Double, 4 },
        }}),
        // Method 'formatRate'
        QtMocHelpers::MethodData<QString(double) const>(5, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Double, 6 },
        }}),
        // Method 'formatPercent'
        QtMocHelpers::MethodData<QString(double, int) const>(7, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Double, 8 }, { QMetaType::Int, 9 },
        }}),
        // Method 'formatPercent'
        QtMocHelpers::MethodData<QString(double) const>(7, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::QString, {{
            { QMetaType::Double, 8 },
        }}),
        // Method 'formatUptime'
        QtMocHelpers::MethodData<QString(qint64) const>(10, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::LongLong, 11 },
        }}),
        // Method 'setSampleInterval'
        QtMocHelpers::MethodData<void(int)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Method 'refresh'
        QtMocHelpers::MethodData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'clearHistory'
        QtMocHelpers::MethodData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'exportReport'
        QtMocHelpers::MethodData<QString()>(16, 2, QMC::AccessPublic, QMetaType::QString),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'cpuPercent'
        QtMocHelpers::PropertyData<double>(17, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'perCore'
        QtMocHelpers::PropertyData<QVariantList>(18, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'memUsed'
        QtMocHelpers::PropertyData<double>(20, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'memTotal'
        QtMocHelpers::PropertyData<double>(21, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'memPercent'
        QtMocHelpers::PropertyData<double>(22, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'swapUsed'
        QtMocHelpers::PropertyData<double>(23, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'swapTotal'
        QtMocHelpers::PropertyData<double>(24, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'netUp'
        QtMocHelpers::PropertyData<double>(25, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'netDown'
        QtMocHelpers::PropertyData<double>(26, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'interfaces'
        QtMocHelpers::PropertyData<QVariantList>(27, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'diskRead'
        QtMocHelpers::PropertyData<double>(28, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'diskWrite'
        QtMocHelpers::PropertyData<double>(29, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'mounts'
        QtMocHelpers::PropertyData<QVariantList>(30, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'cpuTemp'
        QtMocHelpers::PropertyData<double>(31, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'memTemp'
        QtMocHelpers::PropertyData<double>(32, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'sensors'
        QtMocHelpers::PropertyData<QVariantList>(33, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'batteryPercent'
        QtMocHelpers::PropertyData<double>(34, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'batteryRate'
        QtMocHelpers::PropertyData<double>(35, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'batteryState'
        QtMocHelpers::PropertyData<QString>(36, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'cpuHistory'
        QtMocHelpers::PropertyData<QVariantList>(37, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'memHistory'
        QtMocHelpers::PropertyData<QVariantList>(38, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'netUpHistory'
        QtMocHelpers::PropertyData<QVariantList>(39, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'netDownHistory'
        QtMocHelpers::PropertyData<QVariantList>(40, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'diskReadHistory'
        QtMocHelpers::PropertyData<QVariantList>(41, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'diskWriteHistory'
        QtMocHelpers::PropertyData<QVariantList>(42, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'cpuTempHistory'
        QtMocHelpers::PropertyData<QVariantList>(43, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'memTempHistory'
        QtMocHelpers::PropertyData<QVariantList>(44, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'uptimeSeconds'
        QtMocHelpers::PropertyData<qint64>(45, QMetaType::LongLong, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SystemMonitor, qt_meta_tag_ZN13SystemMonitorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SystemMonitor::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SystemMonitorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SystemMonitorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13SystemMonitorE_t>.metaTypes,
    nullptr
} };

void SystemMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SystemMonitor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->statsChanged(); break;
        case 1: { QString _r = _t->formatBytes((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 2: { QString _r = _t->formatRate((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->formatPercent((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 4: { QString _r = _t->formatPercent((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 5: { QString _r = _t->formatUptime((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->setSampleInterval((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->refresh(); break;
        case 8: _t->clearHistory(); break;
        case 9: { QString _r = _t->exportReport();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SystemMonitor::*)()>(_a, &SystemMonitor::statsChanged, 0))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<double*>(_v) = _t->cpuPercent(); break;
        case 1: *reinterpret_cast<QVariantList*>(_v) = _t->perCore(); break;
        case 2: *reinterpret_cast<double*>(_v) = _t->memUsed(); break;
        case 3: *reinterpret_cast<double*>(_v) = _t->memTotal(); break;
        case 4: *reinterpret_cast<double*>(_v) = _t->memPercent(); break;
        case 5: *reinterpret_cast<double*>(_v) = _t->swapUsed(); break;
        case 6: *reinterpret_cast<double*>(_v) = _t->swapTotal(); break;
        case 7: *reinterpret_cast<double*>(_v) = _t->netUp(); break;
        case 8: *reinterpret_cast<double*>(_v) = _t->netDown(); break;
        case 9: *reinterpret_cast<QVariantList*>(_v) = _t->interfaces(); break;
        case 10: *reinterpret_cast<double*>(_v) = _t->diskRead(); break;
        case 11: *reinterpret_cast<double*>(_v) = _t->diskWrite(); break;
        case 12: *reinterpret_cast<QVariantList*>(_v) = _t->mounts(); break;
        case 13: *reinterpret_cast<double*>(_v) = _t->cpuTemp(); break;
        case 14: *reinterpret_cast<double*>(_v) = _t->memTemp(); break;
        case 15: *reinterpret_cast<QVariantList*>(_v) = _t->sensors(); break;
        case 16: *reinterpret_cast<double*>(_v) = _t->batteryPercent(); break;
        case 17: *reinterpret_cast<double*>(_v) = _t->batteryRate(); break;
        case 18: *reinterpret_cast<QString*>(_v) = _t->batteryState(); break;
        case 19: *reinterpret_cast<QVariantList*>(_v) = _t->cpuHistory(); break;
        case 20: *reinterpret_cast<QVariantList*>(_v) = _t->memHistory(); break;
        case 21: *reinterpret_cast<QVariantList*>(_v) = _t->netUpHistory(); break;
        case 22: *reinterpret_cast<QVariantList*>(_v) = _t->netDownHistory(); break;
        case 23: *reinterpret_cast<QVariantList*>(_v) = _t->diskReadHistory(); break;
        case 24: *reinterpret_cast<QVariantList*>(_v) = _t->diskWriteHistory(); break;
        case 25: *reinterpret_cast<QVariantList*>(_v) = _t->cpuTempHistory(); break;
        case 26: *reinterpret_cast<QVariantList*>(_v) = _t->memTempHistory(); break;
        case 27: *reinterpret_cast<qint64*>(_v) = _t->uptimeSeconds(); break;
        default: break;
        }
    }
}

const QMetaObject *SystemMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SystemMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SystemMonitorE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SystemMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}

// SIGNAL 0
void SystemMonitor::statsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
