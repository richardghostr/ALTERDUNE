// include/Game.h - main game class
#ifndef ALTERDUNE_GAME_H
#define ALTERDUNE_GAME_H

#include "Player.h"
#include "Bestiary.h"
#include <memory>
#include <vector>

#include "Monster.h"

class Game {
public:
	Game() = default;
	~Game() = default;

	bool loadData(const std::string &itemsFile, const std::string &monstersFile);
	void run();
	void startCombat();

	// Accessors for tests / GUI
	Player *getPlayer();
	Bestiary &getBestiary();

private:
	void showSummary();
	void checkEnding();

	std::unique_ptr<Player> player_;
	Bestiary bestiary_;
	std::vector<std::unique_ptr<Monster>> monsters_;
	bool gameOver_{false};
};

#endif // ALTERDUNE_GAME_H