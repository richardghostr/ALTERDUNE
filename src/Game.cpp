// src/Game.cpp
#include "../include/Game.h"
#include "../include/FileLoader.h"
#include "../include/ActAction.h"
#include "../include/Normal.h"
#include "../include/MiniBoss.h"
#include "../include/Boss.h"
#include <iostream>
#include <random>

using namespace std;

// ─────────────────────────────────────────────
// Utilitaire
// ─────────────────────────────────────────────
static std::string categoryToString(MonsterCategory c) {
	switch (c) {
		case MonsterCategory::NORMAL:  return "NORMAL";
		case MonsterCategory::MINIBOSS: return "MINIBOSS";
		case MonsterCategory::BOSS:    return "BOSS";
	}
	return "UNKNOWN";
}

// ─────────────────────────────────────────────
// Chargement des données
// ─────────────────────────────────────────────
bool Game::loadData(const std::string &itemsFile, const std::string &monstersFile) {
	std::vector<Item> items;
	if (!FileLoader::loadItems(itemsFile, items)) {
		std::cerr << "[ERREUR] Impossible d'ouvrir le fichier items : " << itemsFile << "\n";
		return false;
	}

	std::vector<std::unique_ptr<Monster>> monsters;
	if (!FileLoader::loadMonsters(monstersFile, monsters)) {
		std::cerr << "[ERREUR] Impossible d'ouvrir le fichier monstres : " << monstersFile << "\n";
		return false;
	}

	player_ = std::make_unique<Player>("", 100);
	for (const auto &it : items) player_->addItem(it);
	monsters_ = std::move(monsters);
	return true;
}

// ─────────────────────────────────────────────
// Résumé initial
// ─────────────────────────────────────────────
void Game::showSummary() {
	cout << "\n╔══════════════════════════════╗\n";
	cout << "  Bienvenue, " << player_->getName() << " !\n";
	cout << "  HP : " << player_->getHP() << " / " << player_->getHPMax() << "\n";
	cout << "  Inventaire :\n";
	const auto &items = player_->listItems();
	if (items.empty()) {
		cout << "    (vide)\n";
	} else {
		for (const auto &it : items)
			cout << "    - " << it.name << " x" << it.quantity
			     << "  (soin : " << it.value << " HP)\n";
	}
	cout << "╚══════════════════════════════╝\n";
}

// ─────────────────────────────────────────────
// Fin de partie
// ─────────────────────────────────────────────
void Game::checkEnding() {
	int kills  = player_->getKills();
	int spares = player_->getSpares();

	cout << "\n╔══════════════════════════════╗\n";
	cout << "  10 VICTOIRES - FIN DE PARTIE\n";
	cout << "╚══════════════════════════════╝\n\n";

	if (kills > 0 && spares == 0) {
		cout << ">>> FIN GENOCIDAIRE <<<\n";
		cout << "Tu as tue tous tes adversaires sans pitie.\n";
		cout << "Le monde tremble a ton passage...\n";
	} else if (spares > 0 && kills == 0) {
		cout << ">>> FIN PACIFISTE <<<\n";
		cout << "Tu as epargne chaque creature rencontree.\n";
		cout << "La paix regne dans ALTERDUNE.\n";
	} else {
		cout << ">>> FIN NEUTRE <<<\n";
		cout << "Tu as tue " << kills << " creature(s) et epargne " << spares << ".\n";
		cout << "Un equilibre fragile entre clemence et violence.\n";
	}

	cout << "\nMerci d'avoir joue a ALTERDUNE !\n";
	gameOver_ = true;
}

// ─────────────────────────────────────────────
// Boucle principale
// ─────────────────────────────────────────────
void Game::run() {
	if (!player_) player_ = std::make_unique<Player>("", 100);

	cout << "╔══════════════════════════════╗\n";
	cout << "       ALTERDUNE RPG\n";
	cout << "╚══════════════════════════════╝\n";
	cout << "Entrez le nom de votre personnage : ";
	std::string name;
	std::getline(std::cin, name);
	if (name.empty()) name = "Inconnu";
	player_->setName(name);

	showSummary();

	while (!gameOver_) {
		int victories = player_->getKills() + player_->getSpares();

		// Vérification fin de partie
		if (victories >= 10) {
			checkEnding();
			break;
		}

		cout << "\n=== MENU PRINCIPAL === (Victoires : " << victories << "/10)\n";
		cout << "1) Bestiaire\n2) Demarrer un combat\n3) Statistiques\n4) Items\n5) Quitter\n";
		cout << "Choix : ";
		std::string choice;
		std::getline(std::cin, choice);

		if (choice == "1") {
			// ── Bestiaire ──
			const auto &entries = bestiary_.entries();
			if (entries.empty()) {
				cout << "Bestiaire vide. Aucun monstre vaincu pour l'instant.\n";
			} else {
				cout << "\n--- Bestiaire (" << entries.size() << " entree(s)) ---\n";
				for (const auto &e : entries) {
					cout << "  " << e.name
					     << " [" << e.category << "]"
					     << "  HP:" << e.hpMax
					     << "  ATK:" << e.atk
					     << "  DEF:" << e.def
					     << "  -> " << (e.spared ? "Epargne" : "Tue") << "\n";
				}
			}

		} else if (choice == "2") {
			// ── Combat ──
			startCombat();

		} else if (choice == "3") {
			// ── Statistiques ──
			int v = player_->getKills() + player_->getSpares();
			cout << "\n--- Statistiques ---\n";
			cout << "  Nom       : " << player_->getName() << "\n";
			cout << "  HP        : " << player_->getHP() << " / " << player_->getHPMax() << "\n";
			cout << "  Tues      : " << player_->getKills() << "\n";
			cout << "  Epargnes  : " << player_->getSpares() << "\n";
			cout << "  Victoires : " << v << " / 10\n";

		} else if (choice == "4") {
			// ── Items ──
			const auto &items = player_->listItems();
			if (items.empty()) {
				cout << "Inventaire vide.\n";
			} else {
				cout << "\n--- Inventaire ---\n";
				for (const auto &it : items)
					cout << "  " << it.name
					     << "  x" << it.quantity
					     << "  (soin : " << it.value << " HP)\n";
				cout << "Utiliser un item ? (nom ou vide pour annuler) : ";
				std::string itemName;
				std::getline(std::cin, itemName);
				if (!itemName.empty()) {
					if (player_->useItem(itemName))
						cout << "Item utilise. HP : " << player_->getHP()
						     << "/" << player_->getHPMax() << "\n";
					else
						cout << "Impossible d'utiliser cet item (nom incorrect ou quantite nulle).\n";
				}
			}

		} else if (choice == "5") {
			cout << "Au revoir, " << player_->getName() << " !\n";
			gameOver_ = true;

		} else {
			cout << "Choix invalide. Entrez 1, 2, 3, 4 ou 5.\n";
		}
	}
}

// ─────────────────────────────────────────────
// Combat
// ─────────────────────────────────────────────
void Game::startCombat() {
	if (monsters_.empty()) {
		cout << "Aucun monstre disponible.\n";
		return;
	}

	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_int_distribution<> dist(0, (int)monsters_.size() - 1);
	Monster *tmpl = monsters_[dist(gen)].get();

	// Créer une copie fraîche du monstre
	std::unique_ptr<Monster> enemy;
	if (tmpl->category() == MonsterCategory::NORMAL)
		enemy = std::make_unique<Normal>(tmpl->getName(), tmpl->getHPMax(), tmpl->getATK(), tmpl->getDEF(), tmpl->getMercyGoal());
	else if (tmpl->category() == MonsterCategory::MINIBOSS)
		enemy = std::make_unique<MiniBoss>(tmpl->getName(), tmpl->getHPMax(), tmpl->getATK(), tmpl->getDEF(), tmpl->getMercyGoal());
	else
		enemy = std::make_unique<Boss>(tmpl->getName(), tmpl->getHPMax(), tmpl->getATK(), tmpl->getDEF(), tmpl->getMercyGoal());
	enemy->setActIds(tmpl->actIds());

	cout << "\n*** Un ennemi apparait : "
	     << enemy->getName()
	     << " [" << categoryToString(enemy->category()) << "]"
	     << "  HP:" << enemy->getHP() << " ***\n";

	bool combatOver = false;

	while (!combatOver && player_->isAlive() && enemy->isAlive()) {
		cout << "\n  [Joueur: " << player_->getHP() << "/" << player_->getHPMax() << " HP]"
		     << "  [" << enemy->getName() << ": " << enemy->getHP() << "/" << enemy->getHPMax() << " HP]"
		     << "  [Mercy: " << enemy->getMercy() << "/" << enemy->getMercyGoal() << "]\n";
		cout << "  1) FIGHT   2) ACT   3) ITEM   4) MERCY\n";
		cout << "  Choix : ";
		std::string choice;
		std::getline(std::cin, choice);

		// ── FIGHT ──
		if (choice == "1") {
			// Draw damage up to the enemy's CURRENT HP (prevents overkill beyond current HP)
			std::uniform_int_distribution<> dmgDist(0, std::max(0, enemy->getHP()));
			int dmg = dmgDist(gen);
			if (dmg == 0)
				cout << "  Votre attaque rate !\n";
			else {
				enemy->takeDamage(dmg);
				cout << "  Vous infligez " << dmg << " degats."
				     << " (HP ennemi : " << enemy->getHP() << ")\n";
			}
			if (!enemy->isAlive()) {
				cout << "  " << enemy->getName() << " est vaincu !\n";
				player_->incKills();
				BestiaryEntry e;
				e.name = enemy->getName();
				e.category = categoryToString(enemy->category());
				e.hpMax = enemy->getHPMax();
				e.atk = enemy->getATK();
				e.def = enemy->getDEF();
				e.spared = false;
				bestiary_.add(e);
				combatOver = true;
			}

		// ── ACT ──
		} else if (choice == "2") {
			auto ids = enemy->actIds();
			int maxActs = std::min(enemy->actCount(), (int)ids.size());
			cout << "  Actions disponibles :\n";
			for (int i = 0; i < maxActs; ++i) {
				const ActAction *a = findActById(ids[i]);
				if (a) cout << "    " << (i+1) << ") " << a->id
				            << "  (Mercy " << (a->mercyImpact >= 0 ? "+" : "") << a->mercyImpact << ")\n";
				else   cout << "    " << (i+1) << ") " << ids[i] << " (inconnue)\n";
			}
			cout << "  Choix action : ";
			std::string actChoice;
			std::getline(std::cin, actChoice);
			int ai = -1;
			try { ai = std::stoi(actChoice) - 1; } catch (...) {}
			if (ai >= 0 && ai < maxActs) {
				const ActAction *a = findActById(ids[ai]);
				if (a) {
					cout << "\n  >> " << a->text << "\n";
					enemy->modifyMercy(a->mercyImpact);
					cout << "  Mercy : " << enemy->getMercy() << "/" << enemy->getMercyGoal() << "\n";
				} else cout << "  Action inconnue.\n";
			} else cout << "  Choix invalide.\n";

		// ── ITEM ──
		} else if (choice == "3") {
			const auto &items = player_->listItems();
			if (items.empty()) {
				cout << "  Pas d'items disponibles.\n";
			} else {
				for (const auto &it : items)
					cout << "  " << it.name << " x" << it.quantity
					     << "  (soin : " << it.value << " HP)\n";
				cout << "  Nom de l'item (vide pour annuler) : ";
				std::string in;
				std::getline(std::cin, in);
				if (!in.empty()) {
					if (player_->useItem(in))
						cout << "  Item utilise. HP joueur : "
						     << player_->getHP() << "/" << player_->getHPMax() << "\n";
					else
						cout << "  Impossible d'utiliser cet item.\n";
				}
			}

		// ── MERCY ──
		} else if (choice == "4") {
			if (enemy->getMercy() >= enemy->getMercyGoal()) {
				cout << "  Vous epargnez " << enemy->getName() << " !\n";
				player_->incSpares();
				BestiaryEntry e;
				e.name = enemy->getName();
				e.category = categoryToString(enemy->category());
				e.hpMax = enemy->getHPMax();
				e.atk = enemy->getATK();
				e.def = enemy->getDEF();
				e.spared = true;
				bestiary_.add(e);
				combatOver = true;
			} else {
				cout << "  Mercy insuffisante ("
				     << enemy->getMercy() << "/" << enemy->getMercyGoal()
				     << "). Utilisez des actions ACT d'abord.\n";
			}

		} else {
			cout << "  Choix invalide.\n";
		}

		// ── Tour du monstre ──
		if (!combatOver && enemy->isAlive()) {
			// Monster damage draws up to the player's CURRENT HP
			std::uniform_int_distribution<> dmgDist2(0, std::max(0, player_->getHP()));
			int dmg = dmgDist2(gen);
			cout << "\n  -- Tour du monstre --\n";
			if (dmg == 0)
				cout << "  " << enemy->getName() << " rate son attaque.\n";
			else {
				player_->takeDamage(dmg);
				cout << "  " << enemy->getName() << " inflige " << dmg
				     << " degats. (HP joueur : " << player_->getHP() << ")\n";
			}
			if (!player_->isAlive()) {
				cout << "\n  Vous etes mort. Fin de la partie.\n";
				gameOver_ = true;
				combatOver = true;
			}
		}
	}

	if (!gameOver_) {
		int victories = player_->getKills() + player_->getSpares();
		cout << "\n  Victoires totales : " << victories << "/10\n";
	}
}

// ─────────────────────────────────────────────
// Accessors
// ─────────────────────────────────────────────
Player  *Game::getPlayer()       { return player_.get(); }
Bestiary &Game::getBestiary()    { return bestiary_; }