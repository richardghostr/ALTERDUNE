// include/Boss.h - BOSS monster (4 ACT)
#ifndef ALTERDUNE_BOSS_H
#define ALTERDUNE_BOSS_H

#include "Monster.h"

class Boss : public Monster {
public:
	Boss() = default;
	Boss(const std::string &name, int hpMax, int atk, int def, int mercyGoal = 100);
	~Boss() override = default;

	int actCount() const override;
};

#endif // ALTERDUNE_BOSS_H

