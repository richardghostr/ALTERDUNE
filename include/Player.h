// include/Player.h - player class
#ifndef ALTERDUNE_PLAYER_H
#define ALTERDUNE_PLAYER_H

#include "Entity.h"
#include "Item.h"
#include "Inventory.h"
#include <memory>
#include <string>
#include <vector>

class Player : public Entity {
public:
	Player() = default;
	Player(const std::string &name, int hpMax);
	~Player() override = default;

	// use an item by name; returns true if used
	bool useItem(const std::string &itemId);
	void addItem(const Item &it);
	const std::vector<Item> &listItems() const;

	void setName(const std::string &name);
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

