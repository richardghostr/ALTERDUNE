// src/Player.cpp - Player implementation
#include "../include/Player.h"
#include "../include/Inventory.h"

Player::Player(const std::string &name, int hpMax)
	: Entity(name, hpMax), inventory_(std::make_unique<Inventory>()) {}

bool Player::useItem(const std::string &itemId) {
	if (!inventory_) return false;
	const Item *it = inventory_->getItem(itemId);
	if (!it) return false;
	if (!inventory_->useItem(itemId)) return false;
	if (it->type == ItemType::HEAL) heal(it->value);
	return true;
}

void Player::addItem(const Item &it) {
	if (!inventory_) inventory_ = std::make_unique<Inventory>();
	inventory_->addItem(it);
}

const std::vector<Item> &Player::listItems() const {
	static const std::vector<Item> empty;
	if (!inventory_) return empty;
	return inventory_->items();
}

void Player::setName(const std::string &name) { name_ = name; }

void Player::heal(int amount) { hp_ += amount; if (hp_ > hpMax_) hp_ = hpMax_; }

int Player::getKills() const { return kills_; }
int Player::getSpares() const { return spares_; }

void Player::incKills() { ++kills_; }
void Player::incSpares() { ++spares_; }

