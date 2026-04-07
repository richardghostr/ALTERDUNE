// src/Player.cpp - Player implementation
#include "../include/Player.h"
#include "../include/Inventory.h"

Player::Player(const std::string &name, int hpMax)
	: Entity(name, hpMax), inventory_(std::make_unique<Inventory>()) {}

void Player::useItem(const std::string &itemId) {
	if (inventory_) inventory_->useItem(itemId);
}

void Player::heal(int amount) { hp_ += amount; if (hp_ > hpMax_) hp_ = hpMax_; }

int Player::getKills() const { return kills_; }
int Player::getSpares() const { return spares_; }

