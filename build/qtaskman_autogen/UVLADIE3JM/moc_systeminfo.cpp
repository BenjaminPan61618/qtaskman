/****************************************************************************
** Meta object code from reading C++ file 'systeminfo.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/systeminfo.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'systeminfo.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10SystemInfoE_t {};
} // unnamed namespace

template <> constexpr inline auto SystemInfo::qt_create_metaobjectdata<qt_meta_tag_ZN10SystemInfoE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SystemInfo",
        "distro",
        "kernel",
        "arch",
        "bits",
        "memory",
        "gpu",
        "sessionType",
        "desktop",
        "hostname",
        "cpuModel",
        "cpuCores"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'distro'
        QtMocHelpers::PropertyData<QString>(1, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'kernel'
        QtMocHelpers::PropertyData<QString>(2, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'arch'
        QtMocHelpers::PropertyData<QString>(3, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'bits'
        QtMocHelpers::PropertyData<QString>(4, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'memory'
        QtMocHelpers::PropertyData<QString>(5, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'gpu'
        QtMocHelpers::PropertyData<QString>(6, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'sessionType'
        QtMocHelpers::PropertyData<QString>(7, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'desktop'
        QtMocHelpers::PropertyData<QString>(8, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'hostname'
        QtMocHelpers::PropertyData<QString>(9, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'cpuModel'
        QtMocHelpers::PropertyData<QString>(10, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'cpuCores'
        QtMocHelpers::PropertyData<int>(11, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SystemInfo, qt_meta_tag_ZN10SystemInfoE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SystemInfo::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SystemInfoE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SystemInfoE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10SystemInfoE_t>.metaTypes,
    nullptr
} };

void SystemInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SystemInfo *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->distro(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->kernel(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->arch(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->bits(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->memory(); break;
        case 5: *reinterpret_cast<QString*>(_v) = _t->gpu(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->sessionType(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->desktop(); break;
        case 8: *reinterpret_cast<QString*>(_v) = _t->hostname(); break;
        case 9: *reinterpret_cast<QString*>(_v) = _t->cpuModel(); break;
        case 10: *reinterpret_cast<int*>(_v) = _t->cpuCores(); break;
        default: break;
        }
    }
}

const QMetaObject *SystemInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SystemInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SystemInfoE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SystemInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
