/****************************************************************************
** Meta object code from reading C++ file 'processcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/processcontroller.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'processcontroller.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN17ProcessControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto ProcessController::qt_create_metaobjectdata<qt_meta_tag_ZN17ProcessControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ProcessController",
        "lastErrorChanged",
        "",
        "sendSignal",
        "pid",
        "signalNumber",
        "setPriority",
        "niceValue",
        "executablePath",
        "openExecutableLocation",
        "restartProcess",
        "clearError",
        "lastError"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'lastErrorChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'sendSignal'
        QtMocHelpers::MethodData<bool(int, int)>(3, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 4 }, { QMetaType::Int, 5 },
        }}),
        // Method 'setPriority'
        QtMocHelpers::MethodData<bool(int, int)>(6, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 4 }, { QMetaType::Int, 7 },
        }}),
        // Method 'executablePath'
        QtMocHelpers::MethodData<QString(int) const>(8, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'openExecutableLocation'
        QtMocHelpers::MethodData<bool(int)>(9, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'restartProcess'
        QtMocHelpers::MethodData<bool(int)>(10, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'clearError'
        QtMocHelpers::MethodData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'lastError'
        QtMocHelpers::PropertyData<QString>(12, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ProcessController, qt_meta_tag_ZN17ProcessControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ProcessController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ProcessControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ProcessControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17ProcessControllerE_t>.metaTypes,
    nullptr
} };

void ProcessController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProcessController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->lastErrorChanged(); break;
        case 1: { bool _r = _t->sendSignal((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 2: { bool _r = _t->setPriority((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->executablePath((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->openExecutableLocation((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->restartProcess((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->clearError(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ProcessController::*)()>(_a, &ProcessController::lastErrorChanged, 0))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->lastError(); break;
        default: break;
        }
    }
}

const QMetaObject *ProcessController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProcessController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ProcessControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ProcessController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ProcessController::lastErrorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
