/****************************************************************************
** Meta object code from reading C++ file 'processmodel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/processmodel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'processmodel.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12ProcessModelE_t {};
} // unnamed namespace

template <> constexpr inline auto ProcessModel::qt_create_metaobjectdata<qt_meta_tag_ZN12ProcessModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ProcessModel",
        "countChanged",
        "",
        "filterModeChanged",
        "flatModeChanged",
        "onSampled",
        "toggleExpanded",
        "row",
        "expandAll",
        "collapseAll",
        "focusPid",
        "pid",
        "hasProcess",
        "count",
        "filterMode",
        "flatMode",
        "Roles",
        "PidRole",
        "PpidRole",
        "NameRole",
        "CommandRole",
        "UserRole",
        "CpuRole",
        "MemRole",
        "MemPercentRole",
        "NetUpRole",
        "NetDownRole",
        "DiskReadRole",
        "DiskWriteRole",
        "IconNameRole",
        "DepthRole",
        "HasChildrenRole",
        "ExpandedRole",
        "IsSelfRole",
        "NiceRole",
        "StateRole",
        "IsNewRole"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'countChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'filterModeChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'flatModeChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onSampled'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'toggleExpanded'
        QtMocHelpers::MethodData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Method 'expandAll'
        QtMocHelpers::MethodData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'collapseAll'
        QtMocHelpers::MethodData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'focusPid'
        QtMocHelpers::MethodData<int(int)>(10, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 11 },
        }}),
        // Method 'hasProcess'
        QtMocHelpers::MethodData<bool(int) const>(12, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 11 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'count'
        QtMocHelpers::PropertyData<int>(13, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'filterMode'
        QtMocHelpers::PropertyData<int>(14, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'flatMode'
        QtMocHelpers::PropertyData<bool>(15, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'Roles'
        QtMocHelpers::EnumData<enum Roles>(16, 16, QMC::EnumFlags{}).add({
            {   17, Roles::PidRole },
            {   18, Roles::PpidRole },
            {   19, Roles::NameRole },
            {   20, Roles::CommandRole },
            {   21, Roles::UserRole },
            {   22, Roles::CpuRole },
            {   23, Roles::MemRole },
            {   24, Roles::MemPercentRole },
            {   25, Roles::NetUpRole },
            {   26, Roles::NetDownRole },
            {   27, Roles::DiskReadRole },
            {   28, Roles::DiskWriteRole },
            {   29, Roles::IconNameRole },
            {   30, Roles::DepthRole },
            {   31, Roles::HasChildrenRole },
            {   32, Roles::ExpandedRole },
            {   33, Roles::IsSelfRole },
            {   34, Roles::NiceRole },
            {   35, Roles::StateRole },
            {   36, Roles::IsNewRole },
        }),
    };
    return QtMocHelpers::metaObjectData<ProcessModel, qt_meta_tag_ZN12ProcessModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ProcessModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ProcessModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ProcessModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12ProcessModelE_t>.metaTypes,
    nullptr
} };

void ProcessModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProcessModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->countChanged(); break;
        case 1: _t->filterModeChanged(); break;
        case 2: _t->flatModeChanged(); break;
        case 3: _t->onSampled(); break;
        case 4: _t->toggleExpanded((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->expandAll(); break;
        case 6: _t->collapseAll(); break;
        case 7: { int _r = _t->focusPid((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 8: { bool _r = _t->hasProcess((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ProcessModel::*)()>(_a, &ProcessModel::countChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProcessModel::*)()>(_a, &ProcessModel::filterModeChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProcessModel::*)()>(_a, &ProcessModel::flatModeChanged, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->rowCount(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->filterMode(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->flatMode(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setFilterMode(*reinterpret_cast<int*>(_v)); break;
        case 2: _t->setFlatMode(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *ProcessModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProcessModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ProcessModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int ProcessModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ProcessModel::countChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ProcessModel::filterModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ProcessModel::flatModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
