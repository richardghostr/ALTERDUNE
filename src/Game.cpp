// src/Game.cpp - Game implementation (skeleton)
#include "../include/Game.h"
#include "../include/FileLoader.h"
#include "../include/ActAction.h"
#include "../include/Normal.h"
#include "../include/MiniBoss.h"
#include "../include/Boss.h"
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

Player *Game::getPlayer() { return player_.get(); }

Bestiary &Game::getBestiary() { return bestiary_; }

static std::string categoryToString(MonsterCategory c) {
	switch (c) {
		case MonsterCategory::NORMAL: return "NORMAL";
		case MonsterCategory::MINIBOSS: return "MINIBOSS";
		case MonsterCategory::BOSS: return "BOSS";
	}
	return "UNKNOWN";
}

void Game::startCombat() {
	if (monsters_.empty()) {
		cout << "Aucun monstre disponible." << endl;
		return;
	}

	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, (int)monsters_.size() - 1);
	int idx = dist(gen);
	Monster *tmpl = monsters_[idx].get();

	unique_ptr<Monster> enemy;
	if (tmpl->category() == MonsterCategory::NORMAL) {
		enemy = make_unique<Normal>(tmpl->getName(), tmpl->getHPMax(), tmpl->getATK(), tmpl->getDEF(), tmpl->getMercyGoal());
	} else if (tmpl->category() == MonsterCategory::MINIBOSS) {
		enemy = make_unique<MiniBoss>(tmpl->getName(), tmpl->getHPMax(), tmpl->getATK(), tmpl->getDEF(), tmpl->getMercyGoal());
	} else {
		enemy = make_unique<Boss>(tmpl->getName(), tmpl->getHPMax(), tmpl->getATK(), tmpl->getDEF(), tmpl->getMercyGoal());
	}
	enemy->setActIds(tmpl->actIds());

	cout << "Un ennemi apparaît : " << enemy->getName() << " (" << categoryToString(enemy->category()) << ")" << endl;

	while (player_ && player_->isAlive() && enemy->isAlive()) {
		cout << "\n-- Tour du joueur --" << endl;
		cout << "1) FIGHT  2) ACT  3) ITEM  4) MERCY" << endl;
		cout << "Choix: ";
		string choice;
		getline(cin, choice);

		if (choice == "1") {
			std::uniform_int_distribution<> dmgDist(0, enemy->getHPMax());
			int dmg = dmgDist(gen);
			if (dmg == 0) cout << "Votre attaque rate." << endl;
			else {
				enemy->takeDamage(dmg);
				cout << "Vous infligez " << dmg << " dégâts. (HP ennemi: " << enemy->getHP() << ")" << endl;
			}
			if (!enemy->isAlive()) {
				cout << "Monstre tué !" << endl;
				player_->incKills();
				BestiaryEntry e;
				e.name = enemy->getName();
				e.category = categoryToString(enemy->category());
				e.hpMax = enemy->getHPMax();
				e.atk = enemy->getATK();
				e.def = enemy->getDEF();
				e.spared = false;
				bestiary_.add(e);
				break;
			}
		} else if (choice == "2") {
			auto ids = enemy->actIds();
			int maxActs = enemy->actCount();
			cout << "Actions disponibles:" << endl;
			for (int i = 0; i < maxActs && i < (int)ids.size(); ++i) {
				const ActAction *a = findActById(ids[i]);
				if (a) cout << i+1 << ") " << a->id << " - " << a->text << " (MercyImpact:" << a->mercyImpact << ")" << endl;
				else cout << i+1 << ") " << ids[i] << " (UNKNOWN)" << endl;
			}
			cout << "Choix action (num): ";
			string actChoice; getline(cin, actChoice);
			int ai = 0; try { ai = stoi(actChoice) - 1; } catch (...) { ai = -1; }
			if (ai >= 0 && ai < maxActs && ai < (int)ids.size()) {
				const ActAction *a = findActById(ids[ai]);
				if (a) {
					cout << a->text << endl;
					enemy->modifyMercy(a->mercyImpact);
					cout << "Mercy: " << enemy->getMercy() << "/" << enemy->getMercyGoal() << endl;
				} else cout << "Action inconnue." << endl;
			} else cout << "Choix invalide." << endl;
		} else if (choice == "3") {
			const auto &items = player_->listItems();
			if (items.empty()) { cout << "Pas d'items." << endl; }
			else {
				cout << "Items:" << endl;
				for (const auto &it : items) cout << it.name << " x" << it.quantity << endl;
				cout << "Nom de l'item à utiliser (vide pour annuler): ";
				string in; getline(cin, in);
				if (!in.empty()) {
					if (player_->useItem(in)) cout << "Item utilisé. HP joueur: " << player_->getHP() << "/" << player_->getHPMax() << endl;
					else cout << "Impossible d'utiliser cet item." << endl;
				}
			}
		} else if (choice == "4") {
			if (enemy->getMercy() >= enemy->getMercyGoal()) {
				cout << "Vous épargnez le monstre !" << endl;
				player_->incSpares();
				BestiaryEntry e;
				e.name = enemy->getName();
				e.category = categoryToString(enemy->category());
				e.hpMax = enemy->getHPMax();
				e.atk = enemy->getATK();
				e.def = enemy->getDEF();
				e.spared = true;
				bestiary_.add(e);
				break;
			} else {
				cout << "Mercy insuffisante." << endl;
			}
		} else {
			cout << "Choix invalide." << endl;
		}

		if (enemy->isAlive()) {
			cout << "\n-- Tour du monstre --" << endl;
			uniform_int_distribution<> dmgDist2(0, player_->getHPMax());
			int dmg = dmgDist2(gen);
			if (dmg == 0) cout << "Le monstre rate son attaque." << endl;
			else {
				player_->takeDamage(dmg);
				cout << "Le monstre inflige " << dmg << " dégâts. (HP joueur: " << player_->getHP() << ")" << endl;
			}
			if (!player_->isAlive()) {
				cout << "Vous êtes mort. Fin de la partie." << endl;
				exit(0);
			}
		}
	}

	int victories = player_->getKills() + player_->getSpares();
	cout << "Victoires: " << victories << endl;
	if (victories >= 10) {
		int kills = player_->getKills();
		int spares = player_->getSpares();
		if (kills > 0 && spares == 0) cout << "Fin Génocidaire" << endl;
		else if (spares > 0 && kills == 0) cout << "Fin Pacifiste" << endl;
		else cout << "Fin Neutre" << endl;
		cout << "Merci d'avoir joué." << endl;
		exit(0);
	}
}

