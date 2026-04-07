// src/Inventory.cpp - Inventory implementation
#include "../include/Inventory.h"

void Inventory::addItem(const Item &it) { items_.push_back(it); }


const Item *Inventory::getItem(const std::string &name) const {
	for (const auto &it : items_) if (it.name == name) return &it;
	return nullptr;
}

bool Inventory::useItem(const std::string &name) {
	for (auto &it : items_) {
		if (it.name == name && it.quantity > 0) {
			--it.quantity;
			return true;
		}
	}
	return false;
}

const std::vector<Item> &Inventory::items() const { return items_; }

