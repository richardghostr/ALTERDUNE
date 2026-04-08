// src/ActAction.cpp - catalogue of ACT actions
#include "../include/ActAction.h"

static std::vector<ActAction> g_catalogue = {
	{"JOKE",       "Tu lui racontes que les epees ont des sentiments. Il a l'air trouble.", 10},
	{"COMPLIMENT", "Tu lui dis qu'il a les plus beaux ecailles de tout le donjon. Il rougit... probablement.", 15},
	{"INSULT",     "Tu l'appelles 'pixel rate'. Il entre dans une rage cosmique.", -20},
	{"REASON",     "Tu lui expliques calmement que la violence ne resout rien. Il reflechit... lentement.", 12},
	{"DANCE",      "Tu effectues une danse du feu improvisee. Il ne sait pas si c'est une menace ou un spectacle.", 8},
	{"OBSERVE",    "Tu le fixes pendant 10 secondes sans ciller. Il commence a douter de lui-meme.", 5},
	{"DISCUSS",    "Tu engages une conversation sur le sens de la vie. Il semble plus calme maintenant.", 7},
	{"SHOUT",      "Tu cries que ses parents etaient des gobelins de bas etage. Il est furieux.", -10},
	{"OFFER_SNACK","Tu sors un sandwich du sac. Il renifle. L'atmoshere se detend bizarrement.", 18},
	{"PET",        "Tu tentes de lui caresser la tete. Il est desarme par ta naive confiance.", 14}
};

const std::vector<ActAction> &getActCatalogue() { return g_catalogue; }

const ActAction *findActById(const std::string &id) {
	for (const auto &a : g_catalogue) if (a.id == id) return &a;
	return nullptr;
}