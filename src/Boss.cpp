// src/Boss.cpp - Boss implementation
#include "../include/Boss.h"

Boss::Boss(const std::string &name, int hpMax)
	: Monster(name, MonsterCategory::BOSS, hpMax) {}

int Boss::actCount() const { return 4; }

