// src/Normal.cpp - Normal monster implementation
#include "../include/Normal.h"

Normal::Normal(const std::string &name, int hpMax)
	: Monster(name, MonsterCategory::NORMAL, hpMax) {}

int Normal::actCount() const { return 2; }

