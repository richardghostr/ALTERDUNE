// include/Inventory.h - inventory holding items
#ifndef ALTERDUNE_INVENTORY_H
#define ALTERDUNE_INVENTORY_H

#include "Item.h"
#include <vector>

class Inventory {
public:
	Inventory() = default;

	void addItem(const Item &it);
	// Return pointer to item (nullptr if not found)
	const Item *getItem(const std::string &name) const;
	// Use an item: decrement quantity and return true if successful
	bool useItem(const std::string &name);
	const std::vector<Item> &items() const;

private:
	std::vector<Item> items_;
};

#endif // ALTERDUNE_INVENTORY_H

