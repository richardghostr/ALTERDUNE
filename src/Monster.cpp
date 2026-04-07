// src/Monster.cpp - Monster implementation

#include "../include/Monster.h"

Monster::Monster(const std::string &name, MonsterCategory category, int hpMax,
				 int atk, int def, int mercyGoal)
	: Entity(name, hpMax), category_(category), mercy_(0), mercyGoal_(mercyGoal), atk_(atk), def_(def) {}

MonsterCategory Monster::category() const { return category_; }

int Monster::getMercy() const { return mercy_; }
int Monster::getMercyGoal() const { return mercyGoal_; }

void Monster::modifyMercy(int delta) {
	mercy_ += delta;
	if (mercy_ < 0) mercy_ = 0;
	if (mercy_ > mercyGoal_) mercy_ = mercyGoal_;
}

const std::vector<std::string> &Monster::actIds() const { return actIds_; }

int Monster::getATK() const { return atk_; }
int Monster::getDEF() const { return def_; }

void Monster::setActIds(const std::vector<std::string> &ids) { actIds_ = ids; }


