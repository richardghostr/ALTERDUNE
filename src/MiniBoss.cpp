// src/MiniBoss.cpp - MiniBoss implementation
#include "../include/MiniBoss.h"

MiniBoss::MiniBoss(const std::string &name, int hpMax, int atk, int def, int mercyGoal)
	: Monster(name, MonsterCategory::MINIBOSS, hpMax, atk, def, mercyGoal) {}

int MiniBoss::actCount() const { return 3; }

