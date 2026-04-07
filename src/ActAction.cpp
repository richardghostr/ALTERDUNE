// src/ActAction.cpp - catalogue of ACT actions
#include "../include/ActAction.h"

static std::vector<ActAction> g_catalogue = {
	{"JOKE", "Tells a silly joke.", 10},
	{"COMPLIMENT", "Gives a sincere compliment.", 15},
	{"INSULT", "Throws an insult.", -20},
	{"REASON", "Attempts to reason.", 12},
	{"DANCE", "Performs an awkward dance.", 8},
	{"OBSERVE", "Stares meaningfully.", 5},
	{"DISCUSS", "Starts a discussion.", 0},
	{"SHOUT", "Shouts loudly.", -10}
};

const std::vector<ActAction> &getActCatalogue() { return g_catalogue; }

const ActAction *findActById(const std::string &id) {
	for (const auto &a : g_catalogue) if (a.id == id) return &a;
	return nullptr;
}

