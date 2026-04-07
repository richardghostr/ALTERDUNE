// src/FileLoader.cpp - File loader skeleton
#include "../include/FileLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "../include/Normal.h"
#include "../include/MiniBoss.h"
#include "../include/Boss.h"

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
	std::string line;
	while (std::getline(ifs, line)) {
		if (line.empty()) continue;
		std::stringstream ss(line);
		std::vector<std::string> tokens;
		std::string tok;
		while (std::getline(ss, tok, ';')) tokens.push_back(tok);
		if (tokens.size() < 6) {
			std::cerr << "Malformed monsters line (too few fields): " << line << "\n";
			continue;
		}
		std::string cat = tokens[0];
		std::string name = tokens[1];
		int hp = 0, atk = 0, def = 0, mercyGoal = 100;
		try {
			hp = std::stoi(tokens[2]);
			atk = std::stoi(tokens[3]);
			def = std::stoi(tokens[4]);
			mercyGoal = std::stoi(tokens[5]);
		} catch (...) {
			std::cerr << "Invalid number in monsters line: " << line << "\n";
			continue;
		}

		std::vector<std::string> acts;
		for (size_t i = 6; i < tokens.size(); ++i) {
			if (tokens[i].empty() || tokens[i] == "-") continue;
			acts.push_back(tokens[i]);
		}

		MonsterCategory category = MonsterCategory::NORMAL;
		if (cat == "NORMAL") category = MonsterCategory::NORMAL;
		else if (cat == "MINIBOSS") category = MonsterCategory::MINIBOSS;
		else if (cat == "BOSS") category = MonsterCategory::BOSS;
		else {
			std::cerr << "Unknown monster category: " << cat << "\n";
			continue;
		}

		std::unique_ptr<Monster> m;
		if (category == MonsterCategory::NORMAL) {
			m = std::make_unique<Normal>(name, hp, atk, def, mercyGoal);
		} else if (category == MonsterCategory::MINIBOSS) {
			m = std::make_unique<MiniBoss>(name, hp, atk, def, mercyGoal);
		} else {
			m = std::make_unique<Boss>(name, hp, atk, def, mercyGoal);
		}
		if (m) m->setActIds(acts);
		out.push_back(std::move(m));
	}
	return true;
}

