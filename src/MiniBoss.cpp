// src/MiniBoss.cpp - MiniBoss implementation
#include "../include/MiniBoss.h"

MiniBoss::MiniBoss(const std::string &name, int hpMax)
	: Monster(name, MonsterCategory::MINIBOSS, hpMax) {}

int MiniBoss::actCount() const { return 3; }

