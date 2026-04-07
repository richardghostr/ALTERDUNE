// include/Item.h - item definition
#ifndef ALTERDUNE_ITEM_H
#define ALTERDUNE_ITEM_H

#include <string>

enum class ItemType { HEAL };

struct Item {
	std::string name;
	ItemType type{ItemType::HEAL};
	int value{0};
	int quantity{0};
};

#endif // ALTERDUNE_ITEM_H

