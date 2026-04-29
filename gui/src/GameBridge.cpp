#include "GameBridge.h"
#include <algorithm>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

#include "../../include/FileLoader.h"
#include "../../include/ActAction.h"
#include "../../include/Normal.h"
#include "../../include/MiniBoss.h"
#include "../../include/Boss.h"

// ─────────────────────────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────────────────────────
GameBridge::GameBridge(QObject *parent) : QObject(parent) {}

// ─────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────
QString GameBridge::categoryStr(MonsterCategory c) const {
    switch (c) {
        case MonsterCategory::NORMAL:   return "NORMAL";
        case MonsterCategory::MINIBOSS: return "MINIBOSS";
        case MonsterCategory::BOSS:     return "BOSS";
    }
    return "NORMAL";
}

// ─────────────────────────────────────────────────────────────────
// Data loading
// ─────────────────────────────────────────────────────────────────
bool GameBridge::loadData(const QString &itemsPath, const QString &monstersPath) {
    // Resolve candidate paths for items and monsters and log attempts
    auto findExisting = [&](const QString &p) -> QString {
        QStringList candidates;
        candidates << p;
        QString appDir = QCoreApplication::applicationDirPath();
        candidates << QDir(appDir).filePath(p);
        candidates << QDir(appDir).filePath(QDir::cleanPath("../" + p));
        candidates << QDir::current().filePath(p);
        candidates << QDir::current().filePath(QDir::cleanPath("../" + p));
        for (const QString &c : candidates) {
            emit logMessage(QString("Tentative de chargement depuis: %1").arg(c), "system");
            if (QFileInfo::exists(c)) return c;
        }
        return QString();
    };

    QString itemsResolved = findExisting(itemsPath);
    if (itemsResolved.isEmpty()) {
        emit logMessage(QString("Erreur: items non trouves (tentatives: %1, %2)").arg(itemsPath).arg(QCoreApplication::applicationDirPath()), "error");
        return false;
    }

    QString monstersResolved = findExisting(monstersPath);
    if (monstersResolved.isEmpty()) {
        emit logMessage(QString("Erreur: monsters non trouves (tentatives: %1, %2)").arg(monstersPath).arg(QCoreApplication::applicationDirPath()), "error");
        return false;
    }

    std::vector<Item> items;
    if (!FileLoader::loadItems(itemsResolved.toStdString(), items)) {
        emit logMessage(QString("Erreur: impossible de charger %1").arg(itemsResolved), "error");
        return false;
    }
    std::vector<std::unique_ptr<Monster>> mons;
    if (!FileLoader::loadMonsters(monstersResolved.toStdString(), mons)) {
        emit logMessage(QString("Erreur: impossible de charger %1").arg(monstersResolved), "error");
        return false;
    }
    monstersPool_.clear();
    for (auto &m : mons)
        monstersPool_.push_back(std::shared_ptr<Monster>(std::move(m)));

    initialItems_ = items;
    player_ = std::make_unique<Player>("", 100);
    for (const auto &it : initialItems_) player_->addItem(it);

    dataReady_ = true;
    emit logMessage("Donnees chargees avec succes.", "system");
    emit stateChanged();
    return true;
}

// ─────────────────────────────────────────────────────────────────
// Game lifecycle
// ─────────────────────────────────────────────────────────────────
void GameBridge::newGame(const QString &playerName) {
    player_ = std::make_unique<Player>(playerName.toStdString(), 100);
    for (const auto &it : initialItems_) player_->addItem(it);
    bestiary_ = Bestiary();
    enemy_.reset();
    lastDmgPlayer_ = 0;
    lastDmgEnemy_  = 0;
    emit logMessage(QString("Nouvelle partie — Bienvenue, %1 !").arg(playerName), "system");
    emit stateChanged();
}

void GameBridge::resetGame() {
    enemy_.reset();
    bestiary_ = Bestiary();
    lastDmgPlayer_ = 0;
    lastDmgEnemy_  = 0;
    player_ = std::make_unique<Player>("", 100);
    for (const auto &it : initialItems_) player_->addItem(it);
    emit stateChanged();
}

// ─────────────────────────────────────────────────────────────────
// Property getters
// ─────────────────────────────────────────────────────────────────
QString GameBridge::playerName()    const { return player_ ? QString::fromStdString(player_->getName()) : ""; }
int     GameBridge::playerHP()      const { return player_ ? player_->getHP()      : 0; }
int     GameBridge::playerHPMax()   const { return player_ ? player_->getHPMax()   : 100; }
int     GameBridge::playerKills()   const { return player_ ? player_->getKills()   : 0; }
int     GameBridge::playerSpares()  const { return player_ ? player_->getSpares()  : 0; }
int     GameBridge::victories()     const { return player_ ? player_->getKills() + player_->getSpares() : 0; }
bool    GameBridge::playerAlive()   const { return player_ ? player_->isAlive()    : false; }
bool    GameBridge::dataReady()     const { return dataReady_; }
int     GameBridge::lastDmgPlayer() const { return lastDmgPlayer_; }
int     GameBridge::lastDmgEnemy()  const { return lastDmgEnemy_; }

QString GameBridge::enemyName()      const { return enemy_ ? QString::fromStdString(enemy_->getName()) : ""; }
int     GameBridge::enemyHP()        const { return enemy_ ? enemy_->getHP()        : 0; }
int     GameBridge::enemyHPMax()     const { return enemy_ ? enemy_->getHPMax()     : 0; }
int     GameBridge::enemyMercy()     const { return enemy_ ? enemy_->getMercy()     : 0; }
int     GameBridge::enemyMercyGoal() const { return enemy_ ? enemy_->getMercyGoal() : 100; }
QString GameBridge::enemyCategory()  const { return enemy_ ? categoryStr(enemy_->category()) : ""; }
bool    GameBridge::inCombat()       const { return enemy_ != nullptr; }

// ─────────────────────────────────────────────────────────────────
// Lists
// ─────────────────────────────────────────────────────────────────
QVariantList GameBridge::inventory() const {
    QVariantList out;
    if (!player_) return out;
    for (const auto &it : player_->listItems()) {
        QVariantMap m;
        m["name"]  = QString::fromStdString(it.name);
        m["value"] = it.value;
        m["qty"]   = it.quantity;
        out.append(m);
    }
    return out;
}

QVariantList GameBridge::enemyActs() const {
    QVariantList out;
    if (!enemy_) return out;
    qDebug() << "[GameBridge] enemyActs() called; actIds size=" << enemy_->actIds().size();
    auto ids     = enemy_->actIds();
    int  maxActs = std::min(enemy_->actCount(), (int)ids.size());
    for (int i = 0; i < maxActs; ++i) {
        const ActAction *a = findActById(ids[i]);
        QVariantMap m;
        m["id"]    = QString::fromStdString(ids[i]);
        m["text"]  = a ? QString::fromStdString(a->text) : "(inconnue)";
        m["mercy"] = a ? a->mercyImpact : 0;
        out.append(m);
    }
    return out;
}

QVariantList GameBridge::bestiaryEntries() const {
    QVariantList out;
    for (const auto &e : bestiary_.entries()) {
        QVariantMap m;
        m["name"]     = QString::fromStdString(e.name);
        m["category"] = QString::fromStdString(e.category);
        m["hpMax"]    = e.hpMax;
        m["atk"]      = e.atk;
        m["def"]      = e.def;
        m["spared"]   = e.spared;
        out.append(m);
    }
    return out;
}

QVariantList GameBridge::monsterPool() const {
    QVariantList out;
    for (const auto &mp : monstersPool_) {
        QVariantMap m;
        m["name"]     = QString::fromStdString(mp->getName());
        m["category"] = categoryStr(mp->category());
        m["hp"]       = mp->getHPMax();
        m["atk"]      = mp->getATK();
        m["def"]      = mp->getDEF();
        // acts
        auto ids = mp->actIds();
        int  cnt = std::min(mp->actCount(), (int)ids.size());
        QStringList actList;
        for (int i = 0; i < cnt; ++i) actList << QString::fromStdString(ids[i]);
        m["acts"] = actList;
        out.append(m);
    }
    return out;
}

// ─────────────────────────────────────────────────────────────────
// Combat — start
// ─────────────────────────────────────────────────────────────────
static std::shared_ptr<Monster> cloneMonster(const Monster *tmpl) {
    if (tmpl->category() == MonsterCategory::NORMAL)
        return std::make_shared<Normal>(tmpl->getName(), tmpl->getHPMax(), tmpl->getATK(), tmpl->getDEF(), tmpl->getMercyGoal());
    if (tmpl->category() == MonsterCategory::MINIBOSS)
        return std::make_shared<MiniBoss>(tmpl->getName(), tmpl->getHPMax(), tmpl->getATK(), tmpl->getDEF(), tmpl->getMercyGoal());
    return std::make_shared<Boss>(tmpl->getName(), tmpl->getHPMax(), tmpl->getATK(), tmpl->getDEF(), tmpl->getMercyGoal());
}

void GameBridge::startEncounter() {
    if (monstersPool_.empty() || !player_ || !player_->isAlive()) return;
    std::uniform_int_distribution<int> dist(0, (int)monstersPool_.size() - 1);
    startEncounterWith(dist(rng_));
}

void GameBridge::startEncounterWith(int poolIndex) {
    if (poolIndex < 0 || poolIndex >= (int)monstersPool_.size()) return;
    if (!player_ || !player_->isAlive()) return;
    lastDmgPlayer_ = 0;
    lastDmgEnemy_  = 0;
    Monster *tmpl = monstersPool_[poolIndex].get();
    enemy_ = cloneMonster(tmpl);
    enemy_->setActIds(tmpl->actIds());
    emit logMessage(QString("Un ennemi apparait : %1 [%2]")
                        .arg(QString::fromStdString(enemy_->getName()))
                        .arg(categoryStr(enemy_->category())), "system");
    emit logMessage(QString("Monstre acts count (template) = %1, (instance) = %2")
                        .arg(tmpl->actIds().size()).arg(enemy_->actIds().size()), "system");
    emit encounterStarted();
    emit stateChanged();
}

// ─────────────────────────────────────────────────────────────────
// Combat — internal monster turn
// ─────────────────────────────────────────────────────────────────
void GameBridge::monsterAttack() {
    if (!enemy_ || !player_) return;
    // Monster damage: draw up to player's current HP to avoid overkill
    std::uniform_int_distribution<int> dist(0, std::max(0, player_->getHP()));
    int dmg = dist(rng_);
    lastDmgPlayer_ = dmg;
    if (dmg == 0) {
        emit logMessage(QString("%1 rate son attaque !").arg(QString::fromStdString(enemy_->getName())), "miss");
    } else {
        player_->takeDamage(dmg);
        emit logMessage(QString("%1 vous inflige %2 degats. (HP : %3/%4)")
                            .arg(QString::fromStdString(enemy_->getName()))
                            .arg(dmg).arg(player_->getHP()).arg(player_->getHPMax()), "monster");
        emit damageFlash(true, dmg);
    }
    if (!player_->isAlive()) {
        emit logMessage("Vous etes tombe au combat. Fin de la partie.", "death");
        enemy_.reset();
        emit playerDied();
        emit stateChanged();
    }
}

// ─────────────────────────────────────────────────────────────────
// Combat — end encounter
// ─────────────────────────────────────────────────────────────────
void GameBridge::endEncounter(bool spared) {
    if (!enemy_ || !player_) return;
    BestiaryEntry e;
    e.name     = enemy_->getName();
    e.category = categoryStr(enemy_->category()).toStdString();
    e.hpMax    = enemy_->getHPMax();
    e.atk      = enemy_->getATK();
    e.def      = enemy_->getDEF();
    e.spared   = spared;
    bestiary_.add(e);
    if (spared) player_->incSpares(); else player_->incKills();
    enemy_.reset();
    emit encounterEnded(true);
    emit stateChanged();
    if (victories() >= 10) {
        emit logMessage("10 victoires atteintes ! Fin de la partie.", "system");
        emit gameEnded();
    } else {
        emit logMessage(QString("Victoires : %1 / 10").arg(victories()), "system");
    }
}

// ─────────────────────────────────────────────────────────────────
// Combat — player actions
// ─────────────────────────────────────────────────────────────────
void GameBridge::playerFight() {
    if (!enemy_ || !player_ || !player_->isAlive()) return;
    // Player damage: draw up to enemy's current HP to avoid overkill
    std::uniform_int_distribution<int> dist(0, std::max(0, enemy_->getHP()));
    int dmg = dist(rng_);
    lastDmgEnemy_ = dmg;
    if (dmg == 0) {
        emit logMessage("Votre attaque rate !", "miss");
    } else {
        enemy_->takeDamage(dmg);
        emit logMessage(QString("Vous infligez %1 degats a %2. (HP : %3/%4)")
                            .arg(dmg)
                            .arg(QString::fromStdString(enemy_->getName()))
                            .arg(enemy_->getHP()).arg(enemy_->getHPMax()), "player");
        emit damageFlash(false, dmg);
    }
    if (!enemy_->isAlive()) {
        emit logMessage(QString("%1 est vaincu !").arg(QString::fromStdString(enemy_->getName())), "victory");
        endEncounter(false);
        return;
    }
    emit stateChanged();
    monsterAttack();
    emit stateChanged();
}

void GameBridge::playerAct(int actIndex) {
    if (!enemy_ || !player_ || !player_->isAlive()) return;
    auto ids     = enemy_->actIds();
    int  maxActs = std::min(enemy_->actCount(), (int)ids.size());
    if (actIndex < 0 || actIndex >= maxActs) return;

    const ActAction *a = findActById(ids[actIndex]);
    if (!a) {
        emit logMessage(QString("Action inconnue : %1").arg(QString::fromStdString(ids[actIndex])), "error");
        return;
    }
    emit logMessage(QString("[%1] %2").arg(QString::fromStdString(a->id)).arg(QString::fromStdString(a->text)), "act");
    enemy_->modifyMercy(a->mercyImpact);
    emit logMessage(QString("Mercy : %1 / %2").arg(enemy_->getMercy()).arg(enemy_->getMercyGoal()), "mercy");
    emit stateChanged();
    monsterAttack();
    emit stateChanged();
}

bool GameBridge::playerUseItem(const QString &itemName) {
    if (!player_) return false;
    bool ok = player_->useItem(itemName.toStdString());
    if (ok) {
        emit logMessage(QString("Vous utilisez %1. HP : %2/%3").arg(itemName).arg(player_->getHP()).arg(player_->getHPMax()), "heal");
    } else {
        emit logMessage(QString("Impossible d'utiliser %1.").arg(itemName), "error");
    }
    // If in combat, monster attacks after item use
    if (ok && enemy_ && enemy_->isAlive()) {
        emit stateChanged();
        monsterAttack();
    }
    emit stateChanged();
    return ok;
}

void GameBridge::playerMercy() {
    if (!enemy_ || !player_ || !player_->isAlive()) return;
    if (enemy_->getMercy() >= enemy_->getMercyGoal()) {
        emit logMessage(QString("Vous epargnez %1 ! Il prend la fuite.").arg(QString::fromStdString(enemy_->getName())), "mercy");
        endEncounter(true);
    } else {
        emit logMessage(QString("Mercy insuffisante (%1/%2). Utilisez ACT d'abord.")
                            .arg(enemy_->getMercy()).arg(enemy_->getMercyGoal()), "error");
        emit stateChanged();
    }
}

void GameBridge::requestStateUpdate() {
    emit logMessage("Requesting state update from QML", "system");
    emit stateChanged();
}

// ─────────────────────────────────────────────────────────────────
// Ending
// ─────────────────────────────────────────────────────────────────
QString GameBridge::endingType() const {
    if (!player_)               return "neutral";
    int k = playerKills(), s = playerSpares();
    if (!player_->isAlive())    return "death";
    if (k > 0 && s == 0)       return "genocide";
    if (s > 0 && k == 0)       return "pacifist";
    return "neutral";
}

QString GameBridge::endingTitle() const {
    QString t = endingType();
    if (t == "death")    return "Defaite";
    if (t == "genocide") return "Fin Genocidaire";
    if (t == "pacifist") return "Fin Pacifiste";
    return "Fin Neutre";
}

QString GameBridge::endingText() const {
    QString t = endingType();
    if (t == "death")    return "Vous etes tombe au combat. ALTERDUNE pleure... ou pas.";
    if (t == "genocide") return "Vous avez tue chaque creature sans un regard en arriere. Le monde vous craint.";
    if (t == "pacifist") return "Vous avez epargne chaque ame rencontree. La paix regne desormais.";
    return "Ni tout blanc ni tout noir. Votre chemin fut celui de l'equilibre.";
}
