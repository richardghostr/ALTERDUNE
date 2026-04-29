// src/main.cpp - simple runner
#include "../include/Game.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#include <locale>
#endif

int main() {
#ifdef _WIN32
    // Ensure Windows console uses UTF-8 so box-drawing and accents display correctly
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::setlocale(LC_ALL, "C.UTF-8");
#endif
    Game g;
    if (!g.loadData("data/items.csv", "data/monsters.csv")) {
        std::cerr << "Failed to load data files.\n";
        return 1;
    }
    g.run();
    return 0;
}
