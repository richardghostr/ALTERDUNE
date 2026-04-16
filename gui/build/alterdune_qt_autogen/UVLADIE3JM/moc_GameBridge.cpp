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
        "stateChanged",
        "",
        "logMessage",
        "msg",
        "type",
        "encounterStarted",
        "encounterEnded",
        "playerWon",
        "playerDied",
        "gameEnded",
        "damageFlash",
        "isPlayer",
        "amount",
        "loadData",
        "itemsPath",
        "monstersPath",
        "newGame",
        "playerName",
        "resetGame",
        "inventory",
        "QVariantList",
        "enemyActs",
        "bestiaryEntries",
        "monsterPool",
        "startEncounter",
        "startEncounterWith",
        "poolIndex",
        "playerFight",
        "playerAct",
        "actIndex",
        "playerUseItem",
        "itemName",
        "playerMercy",
        "endingType",
        "endingTitle",
        "endingText",
        "playerHP",
        "playerHPMax",
        "playerKills",
        "playerSpares",
        "victories",
        "enemyName",
        "enemyHP",
        "enemyHPMax",
        "enemyMercy",
        "enemyMercyGoal",
        "enemyCategory",
        "inCombat",
        "playerAlive",
        "dataReady",
        "lastDmgPlayer",
        "lastDmgEnemy"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'stateChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'logMessage'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 }, { QMetaType::QString, 5 },
        }}),
        // Signal 'encounterStarted'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'encounterEnded'
        QtMocHelpers::SignalData<void(bool)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 8 },
        }}),
        // Signal 'playerDied'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'gameEnded'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'damageFlash'
        QtMocHelpers::SignalData<void(bool, int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 12 }, { QMetaType::Int, 13 },
        }}),
        // Method 'loadData'
        QtMocHelpers::MethodData<bool(const QString &, const QString &)>(14, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 15 }, { QMetaType::QString, 16 },
        }}),
        // Method 'newGame'
        QtMocHelpers::MethodData<void(const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 18 },
        }}),
        // Method 'resetGame'
        QtMocHelpers::MethodData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'inventory'
        QtMocHelpers::MethodData<QVariantList() const>(20, 2, QMC::AccessPublic, 0x80000000 | 21),
        // Method 'enemyActs'
        QtMocHelpers::MethodData<QVariantList() const>(22, 2, QMC::AccessPublic, 0x80000000 | 21),
        // Method 'bestiaryEntries'
        QtMocHelpers::MethodData<QVariantList() const>(23, 2, QMC::AccessPublic, 0x80000000 | 21),
        // Method 'monsterPool'
        QtMocHelpers::MethodData<QVariantList() const>(24, 2, QMC::AccessPublic, 0x80000000 | 21),
        // Method 'startEncounter'
        QtMocHelpers::MethodData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'startEncounterWith'
        QtMocHelpers::MethodData<void(int)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 27 },
        }}),
        // Method 'playerFight'
        QtMocHelpers::MethodData<void()>(28, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'playerAct'
        QtMocHelpers::MethodData<void(int)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 30 },
        }}),
        // Method 'playerUseItem'
        QtMocHelpers::MethodData<bool(const QString &)>(31, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 32 },
        }}),
        // Method 'playerMercy'
        QtMocHelpers::MethodData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'endingType'
        QtMocHelpers::MethodData<QString() const>(34, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'endingTitle'
        QtMocHelpers::MethodData<QString() const>(35, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'endingText'
        QtMocHelpers::MethodData<QString() const>(36, 2, QMC::AccessPublic, QMetaType::QString),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'playerName'
        QtMocHelpers::PropertyData<QString>(18, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'playerHP'
        QtMocHelpers::PropertyData<int>(37, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'playerHPMax'
        QtMocHelpers::PropertyData<int>(38, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'playerKills'
        QtMocHelpers::PropertyData<int>(39, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'playerSpares'
        QtMocHelpers::PropertyData<int>(40, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'victories'
        QtMocHelpers::PropertyData<int>(41, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'enemyName'
        QtMocHelpers::PropertyData<QString>(42, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'enemyHP'
        QtMocHelpers::PropertyData<int>(43, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'enemyHPMax'
        QtMocHelpers::PropertyData<int>(44, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'enemyMercy'
        QtMocHelpers::PropertyData<int>(45, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'enemyMercyGoal'
        QtMocHelpers::PropertyData<int>(46, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'enemyCategory'
        QtMocHelpers::PropertyData<QString>(47, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'inCombat'
        QtMocHelpers::PropertyData<bool>(48, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'playerAlive'
        QtMocHelpers::PropertyData<bool>(49, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'dataReady'
        QtMocHelpers::PropertyData<bool>(50, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'lastDmgPlayer'
        QtMocHelpers::PropertyData<int>(51, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'lastDmgEnemy'
        QtMocHelpers::PropertyData<int>(52, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
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
        case 0: _t->stateChanged(); break;
        case 1: _t->logMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->encounterStarted(); break;
        case 3: _t->encounterEnded((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->playerDied(); break;
        case 5: _t->gameEnded(); break;
        case 6: _t->damageFlash((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 7: { bool _r = _t->loadData((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->newGame((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->resetGame(); break;
        case 10: { QVariantList _r = _t->inventory();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 11: { QVariantList _r = _t->enemyActs();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 12: { QVariantList _r = _t->bestiaryEntries();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 13: { QVariantList _r = _t->monsterPool();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->startEncounter(); break;
        case 15: _t->startEncounterWith((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->playerFight(); break;
        case 17: _t->playerAct((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 18: { bool _r = _t->playerUseItem((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 19: _t->playerMercy(); break;
        case 20: { QString _r = _t->endingType();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 21: { QString _r = _t->endingTitle();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 22: { QString _r = _t->endingText();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GameBridge::*)()>(_a, &GameBridge::stateChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameBridge::*)(const QString & , const QString & )>(_a, &GameBridge::logMessage, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameBridge::*)()>(_a, &GameBridge::encounterStarted, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameBridge::*)(bool )>(_a, &GameBridge::encounterEnded, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameBridge::*)()>(_a, &GameBridge::playerDied, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameBridge::*)()>(_a, &GameBridge::gameEnded, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameBridge::*)(bool , int )>(_a, &GameBridge::damageFlash, 6))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->playerName(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->playerHP(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->playerHPMax(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->playerKills(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->playerSpares(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->victories(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->enemyName(); break;
        case 7: *reinterpret_cast<int*>(_v) = _t->enemyHP(); break;
        case 8: *reinterpret_cast<int*>(_v) = _t->enemyHPMax(); break;
        case 9: *reinterpret_cast<int*>(_v) = _t->enemyMercy(); break;
        case 10: *reinterpret_cast<int*>(_v) = _t->enemyMercyGoal(); break;
        case 11: *reinterpret_cast<QString*>(_v) = _t->enemyCategory(); break;
        case 12: *reinterpret_cast<bool*>(_v) = _t->inCombat(); break;
        case 13: *reinterpret_cast<bool*>(_v) = _t->playerAlive(); break;
        case 14: *reinterpret_cast<bool*>(_v) = _t->dataReady(); break;
        case 15: *reinterpret_cast<int*>(_v) = _t->lastDmgPlayer(); break;
        case 16: *reinterpret_cast<int*>(_v) = _t->lastDmgEnemy(); break;
        default: break;
        }
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
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 23;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void GameBridge::stateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GameBridge::logMessage(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void GameBridge::encounterStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void GameBridge::encounterEnded(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void GameBridge::playerDied()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void GameBridge::gameEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void GameBridge::damageFlash(bool _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2);
}
QT_WARNING_POP
