// include/Monster.h - abstract monster base
#ifndef ALTERDUNE_MONSTER_H
#define ALTERDUNE_MONSTER_H

#include "Entity.h"
#include <string>
#include <vector>

enum class MonsterCategory { NORMAL, MINIBOSS, BOSS };

class Monster : public Entity {
public:
	Monster() = default;
	Monster(const std::string &name, MonsterCategory category, int hpMax,
			int atk, int def, int mercyGoal = 100);
	~Monster() override = default;

	MonsterCategory category() const;

	int getMercy() const;
	int getMercyGoal() const;
	void modifyMercy(int delta);

	int getATK() const;
	int getDEF() const;

	void setActIds(const std::vector<std::string> &ids);

	const std::vector<std::string> &actIds() const;

	virtual int actCount() const = 0; // 2/3/4 depending on subclass

protected:
	MonsterCategory category_{MonsterCategory::NORMAL};
	int mercy_{0};
	int mercyGoal_{100};
	std::vector<std::string> actIds_;
	int atk_{0};
	int def_{0};
};

#endif // ALTERDUNE_MONSTER_H

