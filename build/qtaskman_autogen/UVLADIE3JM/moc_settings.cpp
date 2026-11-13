/****************************************************************************
** Meta object code from reading C++ file 'settings.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/settings.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settings.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN8SettingsE_t {};
} // unnamed namespace

template <> constexpr inline auto Settings::qt_create_metaobjectdata<qt_meta_tag_ZN8SettingsE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Settings",
        "themeModeChanged",
        "",
        "tempUnitChanged",
        "sampleIntervalChanged",
        "stayInTrayChanged",
        "floatOpacityChanged",
        "setThemeMode",
        "mode",
        "setTempUnit",
        "unit",
        "setSampleInterval",
        "ms",
        "setStayInTray",
        "stay",
        "setFloatOpacity",
        "opacity",
        "toDisplayTemp",
        "celsius",
        "tempUnitSymbol",
        "themeMode",
        "tempUnit",
        "sampleInterval",
        "stayInTray",
        "floatOpacity"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'themeModeChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'tempUnitChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sampleIntervalChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'stayInTrayChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'floatOpacityChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'setThemeMode'
        QtMocHelpers::MethodData<void(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Method 'setTempUnit'
        QtMocHelpers::MethodData<void(const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 },
        }}),
        // Method 'setSampleInterval'
        QtMocHelpers::MethodData<void(int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 12 },
        }}),
        // Method 'setStayInTray'
        QtMocHelpers::MethodData<void(bool)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 14 },
        }}),
        // Method 'setFloatOpacity'
        QtMocHelpers::MethodData<void(double)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 16 },
        }}),
        // Method 'toDisplayTemp'
        QtMocHelpers::MethodData<double(double) const>(17, 2, QMC::AccessPublic, QMetaType::Double, {{
            { QMetaType::Double, 18 },
        }}),
        // Method 'tempUnitSymbol'
        QtMocHelpers::MethodData<QString() const>(19, 2, QMC::AccessPublic, QMetaType::QString),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'themeMode'
        QtMocHelpers::PropertyData<QString>(20, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'tempUnit'
        QtMocHelpers::PropertyData<QString>(21, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'sampleInterval'
        QtMocHelpers::PropertyData<int>(22, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'stayInTray'
        QtMocHelpers::PropertyData<bool>(23, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'floatOpacity'
        QtMocHelpers::PropertyData<double>(24, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Settings, qt_meta_tag_ZN8SettingsE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Settings::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8SettingsE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8SettingsE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8SettingsE_t>.metaTypes,
    nullptr
} };

void Settings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Settings *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->themeModeChanged(); break;
        case 1: _t->tempUnitChanged(); break;
        case 2: _t->sampleIntervalChanged(); break;
        case 3: _t->stayInTrayChanged(); break;
        case 4: _t->floatOpacityChanged(); break;
        case 5: _t->setThemeMode((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->setTempUnit((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->setSampleInterval((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->setStayInTray((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 9: _t->setFloatOpacity((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 10: { double _r = _t->toDisplayTemp((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])));
            if (_a[0]) *reinterpret_cast<double*>(_a[0]) = std::move(_r); }  break;
        case 11: { QString _r = _t->tempUnitSymbol();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Settings::*)()>(_a, &Settings::themeModeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Settings::*)()>(_a, &Settings::tempUnitChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Settings::*)()>(_a, &Settings::sampleIntervalChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Settings::*)()>(_a, &Settings::stayInTrayChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Settings::*)()>(_a, &Settings::floatOpacityChanged, 4))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->themeMode(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->tempUnit(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->sampleInterval(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->stayInTray(); break;
        case 4: *reinterpret_cast<double*>(_v) = _t->floatOpacity(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setThemeMode(*reinterpret_cast<QString*>(_v)); break;
        case 1: _t->setTempUnit(*reinterpret_cast<QString*>(_v)); break;
        case 2: _t->setSampleInterval(*reinterpret_cast<int*>(_v)); break;
        case 3: _t->setStayInTray(*reinterpret_cast<bool*>(_v)); break;
        case 4: _t->setFloatOpacity(*reinterpret_cast<double*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *Settings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Settings::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8SettingsE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Settings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Settings::themeModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Settings::tempUnitChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Settings::sampleIntervalChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Settings::stayInTrayChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Settings::floatOpacityChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
