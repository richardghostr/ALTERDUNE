// include/Player.h - player class
#ifndef ALTERDUNE_PLAYER_H
#define ALTERDUNE_PLAYER_H

#include "Entity.h"
#include <memory>
#include <string>

class Inventory;

class Player : public Entity {
public:
	Player() = default;
	Player(const std::string &name, int hpMax);
	~Player() override = default;

	void useItem(const std::string &itemId);
	void heal(int amount);

	// stats accessors
	int getKills() const;
	int getSpares() const;

private:
	std::unique_ptr<Inventory> inventory_;
	int kills_{0};
	int spares_{0};
};

#endif // ALTERDUNE_PLAYER_H

