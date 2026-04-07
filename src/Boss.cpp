// src/Boss.cpp - Boss implementation
#include "../include/Boss.h"

Boss::Boss(const std::string &name, int hpMax, int atk, int def, int mercyGoal)
	: Monster(name, MonsterCategory::BOSS, hpMax, atk, def, mercyGoal) {}

int Boss::actCount() const { return 4; }

