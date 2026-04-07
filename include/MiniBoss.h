// include/MiniBoss.h - MINIBOSS monster (3 ACT)
#ifndef ALTERDUNE_MINIBOSS_H
#define ALTERDUNE_MINIBOSS_H

#include "Monster.h"

class MiniBoss : public Monster {
public:
	MiniBoss() = default;
	MiniBoss(const std::string &name, int hpMax, int atk, int def, int mercyGoal = 100);
	~MiniBoss() override = default;

	int actCount() const override;
};

#endif // ALTERDUNE_MINIBOSS_H

