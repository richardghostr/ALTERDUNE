// include/Inventory.h - inventory holding items
#ifndef ALTERDUNE_INVENTORY_H
#define ALTERDUNE_INVENTORY_H

#include "Item.h"
#include <vector>
#include <optional>

class Inventory {
public:
	Inventory() = default;

	void addItem(const Item &it);
	std::optional<Item> getItem(const std::string &name) const;
	bool useItem(const std::string &name);
	const std::vector<Item> &items() const;

private:
	std::vector<Item> items_;
};

#endif // ALTERDUNE_INVENTORY_H

