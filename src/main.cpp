// src/main.cpp - simple runner
#include "../include/Game.h"
#include <iostream>

int main() {
    Game g;
    if (!g.loadData("data/items.csv", "data/monsters.csv")) {
        std::cerr << "Failed to load data files.\n";
        return 1;
    }
    g.run();
    return 0;
}
