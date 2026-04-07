// include/Entity.h - abstract base class for all entities
#ifndef ALTERDUNE_ENTITY_H
#define ALTERDUNE_ENTITY_H

#include <string>

class Entity {
public:
	Entity() = default;
	Entity(const std::string &name, int hpMax);
	virtual ~Entity() = default;

	const std::string &getName() const;
	int getHP() const;
	int getHPMax() const;
	bool isAlive() const;

	virtual void takeDamage(int dmg);

protected:
	std::string name_;
	int hp_{0};
	int hpMax_{0};
};

#endif // ALTERDUNE_ENTITY_H

