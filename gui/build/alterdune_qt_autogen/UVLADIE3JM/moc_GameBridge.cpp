/****************************************************************************
** Meta object code from reading C++ file 'GameBridge.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/GameBridge.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GameBridge.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
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
struct qt_meta_tag_ZN10GameBridgeE_t {};
} // unnamed namespace

template <> constexpr inline auto GameBridge::qt_create_metaobjectdata<qt_meta_tag_ZN10GameBridgeE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GameBridge",
        "logMessage",
        "",
        "msg",
        "stateChanged",
        "encounterStarted",
        "encounterEnded",
        "loadData",
        "itemsPath",
        "monstersPath",
        "newGame",
        "playerName",
        "playerHP",
        "playerHPMax",
        "victories",
        "inventory",
        "QVariantList",
        "enemyActs",
        "bestiaryEntries",
        "playerKills",
        "playerSpares",
        "startEncounter",
        "enemyName",
        "enemyHP",
        "enemyHPMax",
        "enemyMercy",
        "enemyMercyGoal",
        "playerFight",
        "playerAct",
        "actIndex",
        "playerUseItem",
        "itemName",
        "playerMercy"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'logMessage'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'stateChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'encounterStarted'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'encounterEnded'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'loadData'
        QtMocHelpers::MethodData<bool(const QString &, const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 8 }, { QMetaType::QString, 9 },
        }}),
        // Method 'newGame'
        QtMocHelpers::MethodData<void(const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 },
        }}),
        // Method 'playerName'
        QtMocHelpers::MethodData<QString() const>(11, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'playerHP'
        QtMocHelpers::MethodData<int() const>(12, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'playerHPMax'
        QtMocHelpers::MethodData<int() const>(13, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'victories'
        QtMocHelpers::MethodData<int() const>(14, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'inventory'
        QtMocHelpers::MethodData<QVariantList() const>(15, 2, QMC::AccessPublic, 0x80000000 | 16),
        // Method 'enemyActs'
        QtMocHelpers::MethodData<QVariantList() const>(17, 2, QMC::AccessPublic, 0x80000000 | 16),
        // Method 'bestiaryEntries'
        QtMocHelpers::MethodData<QVariantList() const>(18, 2, QMC::AccessPublic, 0x80000000 | 16),
        // Method 'playerKills'
        QtMocHelpers::MethodData<int() const>(19, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'playerSpares'
        QtMocHelpers::MethodData<int() const>(20, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'startEncounter'
        QtMocHelpers::MethodData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'enemyName'
        QtMocHelpers::MethodData<QString() const>(22, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'enemyHP'
        QtMocHelpers::MethodData<int() const>(23, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'enemyHPMax'
        QtMocHelpers::MethodData<int() const>(24, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'enemyMercy'
        QtMocHelpers::MethodData<int() const>(25, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'enemyMercyGoal'
        QtMocHelpers::MethodData<int() const>(26, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'playerFight'
        QtMocHelpers::MethodData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'playerAct'
        QtMocHelpers::MethodData<void(int)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 29 },
        }}),
        // Method 'playerUseItem'
        QtMocHelpers::MethodData<bool(const QString &)>(30, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 31 },
        }}),
        // Method 'playerMercy'
        QtMocHelpers::MethodData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GameBridge, qt_meta_tag_ZN10GameBridgeE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GameBridge::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GameBridgeE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GameBridgeE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10GameBridgeE_t>.metaTypes,
    nullptr
} };

void GameBridge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GameBridge *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->logMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->stateChanged(); break;
        case 2: _t->encounterStarted(); break;
        case 3: _t->encounterEnded(); break;
        case 4: { bool _r = _t->loadData((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->newGame((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: { QString _r = _t->playerName();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 7: { int _r = _t->playerHP();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 8: { int _r = _t->playerHPMax();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 9: { int _r = _t->victories();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 10: { QVariantList _r = _t->inventory();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 11: { QVariantList _r = _t->enemyActs();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 12: { QVariantList _r = _t->bestiaryEntries();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 13: { int _r = _t->playerKills();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 14: { int _r = _t->playerSpares();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 15: _t->startEncounter(); break;
        case 16: { QString _r = _t->enemyName();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 17: { int _r = _t->enemyHP();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 18: { int _r = _t->enemyHPMax();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 19: { int _r = _t->enemyMercy();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 20: { int _r = _t->enemyMercyGoal();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 21: _t->playerFight(); break;
        case 22: _t->playerAct((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 23: { bool _r = _t->playerUseItem((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 24: _t->playerMercy(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GameBridge::*)(const QString & )>(_a, &GameBridge::logMessage, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameBridge::*)()>(_a, &GameBridge::stateChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameBridge::*)()>(_a, &GameBridge::encounterStarted, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameBridge::*)()>(_a, &GameBridge::encounterEnded, 3))
            return;
    }
}

const QMetaObject *GameBridge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameBridge::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GameBridgeE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GameBridge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void GameBridge::logMessage(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void GameBridge::stateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void GameBridge::encounterStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void GameBridge::encounterEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
