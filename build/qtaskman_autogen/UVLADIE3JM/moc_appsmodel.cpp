/****************************************************************************
** Meta object code from reading C++ file 'appsmodel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/appsmodel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'appsmodel.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9AppsModelE_t {};
} // unnamed namespace

template <> constexpr inline auto AppsModel::qt_create_metaobjectdata<qt_meta_tag_ZN9AppsModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AppsModel",
        "countChanged",
        "",
        "rebuild",
        "refreshValues",
        "onSampled",
        "count",
        "Roles",
        "PidRole",
        "AppNameRole",
        "TitleRole",
        "IconNameRole",
        "CpuRole",
        "MemRole",
        "MemPercentRole",
        "NetUpRole",
        "NetDownRole",
        "DiskReadRole",
        "DiskWriteRole",
        "ActiveWindowRole",
        "PowerRole",
        "StateRole",
        "IsNewRole"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'countChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'rebuild'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'refreshValues'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSampled'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'count'
        QtMocHelpers::PropertyData<int>(6, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'Roles'
        QtMocHelpers::EnumData<enum Roles>(7, 7, QMC::EnumFlags{}).add({
            {    8, Roles::PidRole },
            {    9, Roles::AppNameRole },
            {   10, Roles::TitleRole },
            {   11, Roles::IconNameRole },
            {   12, Roles::CpuRole },
            {   13, Roles::MemRole },
            {   14, Roles::MemPercentRole },
            {   15, Roles::NetUpRole },
            {   16, Roles::NetDownRole },
            {   17, Roles::DiskReadRole },
            {   18, Roles::DiskWriteRole },
            {   19, Roles::ActiveWindowRole },
            {   20, Roles::PowerRole },
            {   21, Roles::StateRole },
            {   22, Roles::IsNewRole },
        }),
    };
    return QtMocHelpers::metaObjectData<AppsModel, qt_meta_tag_ZN9AppsModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AppsModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9AppsModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9AppsModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9AppsModelE_t>.metaTypes,
    nullptr
} };

void AppsModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AppsModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->countChanged(); break;
        case 1: _t->rebuild(); break;
        case 2: _t->refreshValues(); break;
        case 3: _t->onSampled(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (AppsModel::*)()>(_a, &AppsModel::countChanged, 0))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->rowCount(); break;
        default: break;
        }
    }
}

const QMetaObject *AppsModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AppsModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9AppsModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int AppsModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
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
void AppsModel::countChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
