#include "GameBridge.h"
#include <random>
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

GameBridge::GameBridge(QObject *parent) : QObject(parent) {}

bool GameBridge::loadData(const QString &itemsPath, const QString &monstersPath) {
    auto resolveCandidates = [&](const QString &p) {
        QStringList candidates;
        candidates << p;
        // path relative to application dir
        QString appDir = QCoreApplication::applicationDirPath();
        candidates << QDir(appDir).filePath(p);
        // try parent directories (up to two levels)
        candidates << QDir(appDir).filePath(QString("../") + p);
        candidates << QDir(appDir).filePath(QString("../../") + p);
        // also try CWD
        candidates << QDir::current().filePath(p);
        // normalize
        for (auto &c : candidates) c = QDir::cleanPath(c);
        return candidates;
    };

    auto pickExisting = [&](const QString &p) -> QString {
        QStringList cand = resolveCandidates(p);
        for (const QString &c : cand) {
            if (QFileInfo::exists(c)) return c;
        }
        return QString();
    };

    QString ipath = pickExisting(itemsPath);
    if (ipath.isEmpty()) ipath = itemsPath; // fallback to provided
    QString mpath = pickExisting(monstersPath);
    if (mpath.isEmpty()) mpath = monstersPath;

    emit logMessage(QString("Chargement items depuis: %1").arg(ipath));
    emit logMessage(QString("Chargement monsters depuis: %1").arg(mpath));

    std::string ip = ipath.toStdString();
    std::string mp = mpath.toStdString();
    std::vector<Item> items;
    if (!FileLoader::loadItems(ip, items)) {
        emit logMessage(QString::fromUtf8("Erreur: impossible de charger items depuis %1").arg(ipath));
        return false;
    }
    std::vector<std::unique_ptr<Monster>> mons;
    if (!FileLoader::loadMonsters(mp, mons)) {
        emit logMessage(QString::fromUtf8("Erreur: impossible de charger monsters depuis %1").arg(mpath));
        return false;
    }

    // populate monstersPool_
    monstersPool_.clear();
    for (auto &m : mons) {
        // convert unique_ptr -> shared_ptr
        monstersPool_.push_back(std::shared_ptr<Monster>(std::move(m)));
    }

    // create default player with items
    player_ = std::make_unique<Player>("", 100);
    for (const auto &it : items) player_->addItem(it);

    emit logMessage("Données chargées.");
    emit stateChanged();
    return true;
}

void GameBridge::newGame(const QString &playerName) {
    if (!player_) player_ = std::make_unique<Player>("", 100);
    player_->setName(playerName.toStdString());
    emit logMessage(QString::fromUtf8("Nouvelle partie pour %1").arg(playerName));
    emit stateChanged();
}

QString GameBridge::playerName() const { return player_ ? QString::fromStdString(player_->getName()) : QString(); }
int GameBridge::playerHP() const { return player_ ? player_->getHP() : 0; }
int GameBridge::playerHPMax() const { return player_ ? player_->getHPMax() : 0; }
int GameBridge::victories() const { return player_ ? (player_->getKills() + player_->getSpares()) : 0; }

QVariantList GameBridge::inventory() const {
    QVariantList out;
    if (!player_) return out;
    for (const auto &it : player_->listItems()) {
        QVariantMap m;
        m["name"] = QString::fromStdString(it.name);
        m["value"] = it.value;
        m["qty"] = it.quantity;
        out.append(m);
    }
    return out;
}

void GameBridge::startEncounter() {
    if (monstersPool_.empty()) {
        emit logMessage("Pas de monstres charges.");
        return;
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, (int)monstersPool_.size() - 1);
    Monster *tmpl = monstersPool_[dist(gen)].get();

    if (tmpl->category() == MonsterCategory::NORMAL)
        enemy_ = std::make_shared<Normal>(tmpl->getName(), tmpl->getHPMax(), tmpl->getATK(), tmpl->getDEF(), tmpl->getMercyGoal());
    else if (tmpl->category() == MonsterCategory::MINIBOSS)
        enemy_ = std::make_shared<MiniBoss>(tmpl->getName(), tmpl->getHPMax(), tmpl->getATK(), tmpl->getDEF(), tmpl->getMercyGoal());
    else
        enemy_ = std::make_shared<Boss>(tmpl->getName(), tmpl->getHPMax(), tmpl->getATK(), tmpl->getDEF(), tmpl->getMercyGoal());
    enemy_->setActIds(tmpl->actIds());

    emit logMessage(QString::fromUtf8("Un ennemi apparait: %1").arg(QString::fromStdString(enemy_->getName())));
    emit encounterStarted();
    emit stateChanged();
}

QString GameBridge::enemyName() const { return enemy_ ? QString::fromStdString(enemy_->getName()) : QString(); }
int GameBridge::enemyHP() const { return enemy_ ? enemy_->getHP() : 0; }
int GameBridge::enemyHPMax() const { return enemy_ ? enemy_->getHPMax() : 0; }
int GameBridge::enemyMercy() const { return enemy_ ? enemy_->getMercy() : 0; }
int GameBridge::enemyMercyGoal() const { return enemy_ ? enemy_->getMercyGoal() : 100; }

QVariantList GameBridge::enemyActs() const {
    QVariantList out;
    if (!enemy_) return out;
    auto ids = enemy_->actIds();
    int maxActs = std::min(enemy_->actCount(), (int)ids.size());
    for (int i = 0; i < maxActs; ++i) {
        const ActAction *a = findActById(ids[i]);
        QVariantMap m;
        m["id"] = QString::fromStdString(ids[i]);
        if (a) {
            m["text"] = QString::fromStdString(a->text);
            m["mercy"] = a->mercyImpact;
        } else {
            m["text"] = QString::fromUtf8("(inconnue)");
            m["mercy"] = 0;
        }
        out.append(m);
    }
    return out;
}

void GameBridge::playerFight() {
    if (!enemy_ || !player_) return;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dmgDist(0, enemy_->getHPMax());
    int dmg = dmgDist(gen);
    if (dmg == 0) emit logMessage("Votre attaque rate !");
    else {
        enemy_->takeDamage(dmg);
        emit logMessage(QString::fromUtf8("Vous infligez %1 degats.").arg(dmg));
    }
    if (!enemy_->isAlive()) {
        emit logMessage(QString::fromUtf8("%1 est vaincu !").arg(QString::fromStdString(enemy_->getName())));
        player_->incKills();
        BestiaryEntry e;
        e.name = enemy_->getName(); e.category = (enemy_->category()==MonsterCategory::NORMAL?"NORMAL":(enemy_->category()==MonsterCategory::MINIBOSS?"MINIBOSS":"BOSS"));
        e.hpMax = enemy_->getHPMax(); e.atk = enemy_->getATK(); e.def = enemy_->getDEF(); e.spared = false;
        bestiary_.add(e);
        enemy_.reset();
        emit encounterEnded();
    }

    // monster turn if still alive
    if (enemy_ && enemy_->isAlive()) {
        std::uniform_int_distribution<> dmgDist2(0, player_->getHPMax());
        int dmg2 = dmgDist2(gen);
        if (dmg2 == 0) emit logMessage(QString::fromUtf8("%1 rate son attaque.").arg(QString::fromStdString(enemy_->getName())));
        else {
            player_->takeDamage(dmg2);
            emit logMessage(QString::fromUtf8("%1 inflige %2 degats.").arg(QString::fromStdString(enemy_->getName())).arg(dmg2));
        }
        if (!player_->isAlive()) {
            emit logMessage("Vous etes mort. Fin de la partie.");
            // reset or keep state; for GUI we keep game state
        }
    }
    emit stateChanged();
}

void GameBridge::playerAct(int actIndex) {
    if (!enemy_) return;
    auto ids = enemy_->actIds();
    int maxActs = std::min(enemy_->actCount(), (int)ids.size());
    if (actIndex < 0 || actIndex >= maxActs) {
        emit logMessage("Action invalide.");
        return;
    }
    const ActAction *a = findActById(ids[actIndex]);
    if (!a) {
        emit logMessage(QString::fromUtf8("Action inconnue: %1").arg(QString::fromStdString(ids[actIndex])));
        return;
    }
    emit logMessage(QString::fromStdString(a->text));
    enemy_->modifyMercy(a->mercyImpact);
    emit logMessage(QString::fromUtf8("Mercy: %1/%2").arg(enemy_->getMercy()).arg(enemy_->getMercyGoal()));

    // after ACT, monster takes turn
    if (enemy_->isAlive()) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dmgDist2(0, player_->getHPMax());
        int dmg2 = dmgDist2(gen);
        if (dmg2 == 0) emit logMessage(QString::fromUtf8("%1 rate son attaque.").arg(QString::fromStdString(enemy_->getName())));
        else { player_->takeDamage(dmg2); emit logMessage(QString::fromUtf8("%1 inflige %2 degats.").arg(QString::fromStdString(enemy_->getName())).arg(dmg2)); }
        if (!player_->isAlive()) emit logMessage("Vous etes mort. Fin de la partie.");
    }
    emit stateChanged();
}

bool GameBridge::playerUseItem(const QString &itemName) {
    if (!player_) return false;
    bool ok = player_->useItem(itemName.toStdString());
    if (ok) emit logMessage(QString::fromUtf8("Item %1 utilise.").arg(itemName));
    else emit logMessage(QString::fromUtf8("Impossible d'utiliser l'item %1.").arg(itemName));
    emit stateChanged();
    return ok;
}

void GameBridge::playerMercy() {
    if (!enemy_ || !player_) return;
    if (enemy_->getMercy() >= enemy_->getMercyGoal()) {
        emit logMessage(QString::fromUtf8("Vous epargnez %1 !").arg(QString::fromStdString(enemy_->getName())));
        player_->incSpares();
        BestiaryEntry e;
        e.name = enemy_->getName(); e.category = (enemy_->category()==MonsterCategory::NORMAL?"NORMAL":(enemy_->category()==MonsterCategory::MINIBOSS?"MINIBOSS":"BOSS"));
        e.hpMax = enemy_->getHPMax(); e.atk = enemy_->getATK(); e.def = enemy_->getDEF(); e.spared = true;
        bestiary_.add(e);
        enemy_.reset();
        emit encounterEnded();
    } else {
        emit logMessage(QString::fromUtf8("Mercy insuffisante (%1/%2)").arg(enemy_->getMercy()).arg(enemy_->getMercyGoal()));
    }
    emit stateChanged();
}

void GameBridge::emitState(const QString &extra) {
    Q_UNUSED(extra)
    emit stateChanged();
}

QVariantList GameBridge::bestiaryEntries() const {
    QVariantList out;
    for (const auto &e : bestiary_.entries()) {
        QVariantMap m;
        m["name"] = QString::fromStdString(e.name);
        m["category"] = QString::fromStdString(e.category);
        m["hpMax"] = e.hpMax;
        m["atk"] = e.atk;
        m["def"] = e.def;
        m["spared"] = e.spared;
        out.append(m);
    }
    return out;
}

int GameBridge::playerKills() const { return player_ ? player_->getKills() : 0; }
int GameBridge::playerSpares() const { return player_ ? player_->getSpares() : 0; }
