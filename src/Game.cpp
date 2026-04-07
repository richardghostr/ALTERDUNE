// src/Game.cpp - Game implementation (skeleton)
#include "../include/Game.h"
#include "../include/FileLoader.h"
#include <iostream>

#include <limits>
#include <random>

using namespace std;

bool Game::loadData(const std::string &itemsFile, const std::string &monstersFile) {
	std::vector<Item> items;
	if (!FileLoader::loadItems(itemsFile, items)) {
		std::cerr << "Failed to open items file: " << itemsFile << "\n";
		return false;
	}

	std::vector<std::unique_ptr<Monster>> monsters;
	if (!FileLoader::loadMonsters(monstersFile, monsters)) {
		std::cerr << "Failed to open monsters file: " << monstersFile << "\n";
		return false;
	}

	// create player with default HP and give initial items
	player_ = std::make_unique<Player>("", 100);
	for (const auto &it : items) player_->addItem(it);

	monsters_ = std::move(monsters);
	return true;
}

void Game::run() {
	if (!player_) player_ = std::make_unique<Player>("", 100);

	std::cout << "Entrez le nom de votre personnage: ";
	std::string name;
	std::getline(std::cin, name);
	if (name.empty()) name = "Player";
	player_->setName(name);

	bool quit = false;
	while (!quit) {
		std::cout << "\n=== ALTERDUNE - Menu Principal ===\n";
		std::cout << "1) Bestiaire\n2) Démarrer un combat\n3) Statistiques\n4) Items\n5) Quitter\n";
		std::cout << "Choix: ";
		std::string choice;
		std::getline(std::cin, choice);

		if (choice == "1") {
			const auto &entries = bestiary_.entries();
			if (entries.empty()) {
				std::cout << "Bestiaire vide.\n";
			} else {
				for (const auto &e : entries) {
					std::cout << e.name << " (" << e.category << ") HP:" << e.hpMax
							  << " ATK:" << e.atk << " DEF:" << e.def
							  << " -> " << (e.spared ? "Épargné" : "Tué") << "\n";
				}
			}
		} else if (choice == "2") {
			startCombat();
		} else if (choice == "3") {
			std::cout << "Nom: " << player_->getName() << "\n";
			std::cout << "HP: " << player_->getHP() << "/" << player_->getHPMax() << "\n";
			std::cout << "Tués: " << player_->getKills() << "\n";
			std::cout << "Épargnés: " << player_->getSpares() << "\n";
			std::cout << "Victoires: " << (player_->getKills() + player_->getSpares()) << "\n";
		} else if (choice == "4") {
			const auto &items = player_->listItems();
			if (items.empty()) {
				std::cout << "Inventaire vide.\n";
			} else {
				for (const auto &it : items) {
					std::cout << it.name << " x" << it.quantity << " (" << it.value << " HP)\n";
				}
				std::cout << "Souhaitez-vous utiliser un item ? (nom ou vide): ";
				std::string itemName;
				std::getline(std::cin, itemName);
				if (!itemName.empty()) {
					if (player_->useItem(itemName)) std::cout << "Item utilisé.\n";
					else std::cout << "Impossible d'utiliser cet item.\n";
				}
			}
		} else if (choice == "5") {
			quit = true;
		} else {
			std::cout << "Choix invalide.\n";
		}
	}
	std::cout << "Au revoir.\n";
}

