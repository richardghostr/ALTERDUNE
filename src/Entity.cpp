// src/Entity.cpp - Entity implementation
#include "../include/Entity.h"

Entity::Entity(const std::string &name, int hpMax)
	: name_(name), hp_(hpMax), hpMax_(hpMax) {}

const std::string &Entity::getName() const { return name_; }
int Entity::getHP() const { return hp_; }
int Entity::getHPMax() const { return hpMax_; }
bool Entity::isAlive() const { return hp_ > 0; }

void Entity::takeDamage(int dmg) {
	if (dmg <= 0) return;
	hp_ -= dmg;
	if (hp_ < 0) hp_ = 0;
}

