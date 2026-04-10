#pragma once
#include <QObject>
#include <QString>
#include <QVariantList>
#include <memory>
#include <vector>

#include "../../include/Player.h"
#include "../../include/Bestiary.h"

// Forward declarations to avoid requiring Monster to be complete in this header
class Monster;

class GameBridge : public QObject {
    Q_OBJECT
public:
    explicit GameBridge(QObject *parent = nullptr);

    Q_INVOKABLE bool loadData(const QString &itemsPath, const QString &monstersPath);
    Q_INVOKABLE void newGame(const QString &playerName);

    Q_INVOKABLE QString playerName() const;
    Q_INVOKABLE int playerHP() const;
    Q_INVOKABLE int playerHPMax() const;
    Q_INVOKABLE int victories() const;

    Q_INVOKABLE QVariantList inventory() const;

    // Enemy / acts
    Q_INVOKABLE QVariantList enemyActs() const;

    // Bestiary
    Q_INVOKABLE QVariantList bestiaryEntries() const;

    // Player stats
    Q_INVOKABLE int playerKills() const;
    Q_INVOKABLE int playerSpares() const;

    // Combat control
    Q_INVOKABLE void startEncounter();
    Q_INVOKABLE QString enemyName() const;
    Q_INVOKABLE int enemyHP() const;
    Q_INVOKABLE int enemyHPMax() const;
    Q_INVOKABLE int enemyMercy() const;
    Q_INVOKABLE int enemyMercyGoal() const;

    Q_INVOKABLE void playerFight();
    Q_INVOKABLE void playerAct(int actIndex);
    Q_INVOKABLE bool playerUseItem(const QString &itemName);
    Q_INVOKABLE void playerMercy();

signals:
    void logMessage(const QString &msg);
    void stateChanged();
    void encounterStarted();
    void encounterEnded();

private:
    std::unique_ptr<Player> player_;
    Bestiary bestiary_;
    std::vector<std::shared_ptr<Monster>> monstersPool_;
    std::shared_ptr<Monster> enemy_; // current encounter

    void emitState(const QString &extra = QString());
};
