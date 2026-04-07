// include/Bestiary.h - records of defeated monsters
#ifndef ALTERDUNE_BESTIARY_H
#define ALTERDUNE_BESTIARY_H

#include <string>
#include <vector>

struct BestiaryEntry {
	std::string name;
	std::string category;
	int hpMax{0};
	int atk{0};
	int def{0};
	bool spared{false};
};

class Bestiary {
public:
	void add(const BestiaryEntry &e);
	const std::vector<BestiaryEntry> &entries() const;

private:
	std::vector<BestiaryEntry> entries_;
};

#endif // ALTERDUNE_BESTIARY_H

