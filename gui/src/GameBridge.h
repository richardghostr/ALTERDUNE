#pragma once
#include <QObject>
#include <QString>
#include <QVariantList>
#include <memory>
#include <vector>
#include <random>

#include "../../include/Player.h"
#include "../../include/Bestiary.h"
#include "../../include/Monster.h"


class GameBridge : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString  playerName    READ playerName    NOTIFY stateChanged)
    Q_PROPERTY(int      playerHP      READ playerHP      NOTIFY stateChanged)
    Q_PROPERTY(int      playerHPMax   READ playerHPMax   NOTIFY stateChanged)
    Q_PROPERTY(int      playerKills   READ playerKills   NOTIFY stateChanged)
    Q_PROPERTY(int      playerSpares  READ playerSpares  NOTIFY stateChanged)
    Q_PROPERTY(int      victories     READ victories     NOTIFY stateChanged)
    Q_PROPERTY(QString  enemyName     READ enemyName     NOTIFY stateChanged)
    Q_PROPERTY(int      enemyHP       READ enemyHP       NOTIFY stateChanged)
    Q_PROPERTY(int      enemyHPMax    READ enemyHPMax    NOTIFY stateChanged)
    Q_PROPERTY(int      enemyMercy    READ enemyMercy    NOTIFY stateChanged)
    Q_PROPERTY(int      enemyMercyGoal READ enemyMercyGoal NOTIFY stateChanged)
    Q_PROPERTY(QString  enemyCategory READ enemyCategory NOTIFY stateChanged)
    Q_PROPERTY(bool     inCombat      READ inCombat      NOTIFY stateChanged)
    Q_PROPERTY(bool     playerAlive   READ playerAlive   NOTIFY stateChanged)
    Q_PROPERTY(bool     dataReady     READ dataReady     NOTIFY stateChanged)
    Q_PROPERTY(int      lastDmgPlayer READ lastDmgPlayer NOTIFY stateChanged)
    Q_PROPERTY(int      lastDmgEnemy  READ lastDmgEnemy  NOTIFY stateChanged)

public:
    explicit GameBridge(QObject *parent = nullptr);

    Q_INVOKABLE bool loadData(const QString &itemsPath, const QString &monstersPath);
    Q_INVOKABLE void newGame(const QString &playerName);
    Q_INVOKABLE void resetGame();

    QString playerName()     const;
    int     playerHP()       const;
    int     playerHPMax()    const;
    int     playerKills()    const;
    int     playerSpares()   const;
    int     victories()      const;
    QString enemyName()      const;
    int     enemyHP()        const;
    int     enemyHPMax()     const;
    int     enemyMercy()     const;
    int     enemyMercyGoal() const;
    QString enemyCategory()  const;
    bool    inCombat()       const;
    bool    playerAlive()    const;
    bool    dataReady()      const;
    int     lastDmgPlayer()  const;
    int     lastDmgEnemy()   const;

    Q_INVOKABLE QVariantList inventory()        const;
    Q_INVOKABLE QVariantList enemyActs()        const;
    Q_INVOKABLE QVariantList bestiaryEntries()  const;
    Q_INVOKABLE QVariantList monsterPool()      const;

    Q_INVOKABLE void startEncounter();
    Q_INVOKABLE void startEncounterWith(int poolIndex);
    Q_INVOKABLE void playerFight();
    Q_INVOKABLE void playerAct(int actIndex);
    Q_INVOKABLE bool playerUseItem(const QString &itemName);
    Q_INVOKABLE void playerMercy();

    Q_INVOKABLE QString endingType()  const;
    Q_INVOKABLE QString endingTitle() const;
    Q_INVOKABLE QString endingText()  const;

signals:
    void stateChanged();
    void logMessage(const QString &msg, const QString &type);
    void encounterStarted();
    void encounterEnded(bool playerWon);
    void playerDied();
    void gameEnded();
    void damageFlash(bool isPlayer, int amount);

private:
    void    monsterAttack();
    void    endEncounter(bool spared);
    QString categoryStr(MonsterCategory c) const;

    std::unique_ptr<Player>               player_;
    Bestiary                              bestiary_;
    std::vector<std::shared_ptr<Monster>> monstersPool_;
    std::shared_ptr<Monster>              enemy_;
    bool                                  dataReady_     { false };
    int                                   lastDmgPlayer_ { 0 };
    int                                   lastDmgEnemy_  { 0 };
    mutable std::mt19937                  rng_           { std::random_device{}() };
};
