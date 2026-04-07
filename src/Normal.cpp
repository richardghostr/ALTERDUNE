// src/Normal.cpp - Normal monster implementation

#include "../include/Normal.h"

Normal::Normal(const std::string &name, int hpMax, int atk, int def, int mercyGoal)
	: Monster(name, MonsterCategory::NORMAL, hpMax, atk, def, mercyGoal) {}

int Normal::actCount() const { return 2; }

