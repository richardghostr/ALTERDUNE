// src/FileLoader.cpp - File loader skeleton
#include "../include/FileLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool FileLoader::loadItems(const std::string &path, std::vector<Item> &out) {
	std::ifstream ifs(path);
	if (!ifs) return false;
	std::string line;
	while (std::getline(ifs, line)) {
		if (line.empty()) continue;
		std::stringstream ss(line);
		std::string name, type, valueStr, qtyStr;
		if (!std::getline(ss, name, ';')) continue;
		if (!std::getline(ss, type, ';')) continue;
		if (!std::getline(ss, valueStr, ';')) continue;
		if (!std::getline(ss, qtyStr, ';')) continue;
		Item it;
		it.name = name;
		it.type = ItemType::HEAL;
		it.value = std::stoi(valueStr);
		it.quantity = std::stoi(qtyStr);
		out.push_back(it);
	}
	return true;
}

bool FileLoader::loadMonsters(const std::string &path, std::vector<std::unique_ptr<Monster>> &out) {
	std::ifstream ifs(path);
	if (!ifs) return false;
	// Full parsing requires constructing correct Monster subclass; omitted here
	(void)out; (void)path;
	return true;
}

