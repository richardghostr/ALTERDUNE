// include/Game.h - main game class
#ifndef ALTERDUNE_GAME_H
#define ALTERDUNE_GAME_H

#include "Player.h"
#include "Bestiary.h"
#include <memory>

class Game {
public:
	Game() = default;
	~Game() = default;

	bool loadData(const std::string &itemsFile, const std::string &monstersFile);
	void run();

private:
	std::unique_ptr<Player> player_;
	Bestiary bestiary_;
};

#endif // ALTERDUNE_GAME_H

