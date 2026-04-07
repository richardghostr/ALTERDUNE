// include/ActAction.h - ACT action definition and catalogue
#ifndef ALTERDUNE_ACTACTION_H
#define ALTERDUNE_ACTACTION_H

#include <string>
#include <vector>

struct ActAction {
	std::string id;
	std::string text;
	int mercyImpact{0}; // positive calms, negative angers
};

// Returns the global catalogue of actions (defined in .cpp)
const std::vector<ActAction> &getActCatalogue();
const ActAction *findActById(const std::string &id);

#endif // ALTERDUNE_ACTACTION_H

