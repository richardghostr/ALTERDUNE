// include/Normal.h - NORMAL monster (2 ACT)
#ifndef ALTERDUNE_NORMAL_H
#define ALTERDUNE_NORMAL_H

#include "Monster.h"

class Normal : public Monster {
public:
	Normal() = default;
	Normal(const std::string &name, int hpMax, int atk, int def, int mercyGoal = 100);
	~Normal() override = default;

	int actCount() const override;
};

#endif // ALTERDUNE_NORMAL_H

