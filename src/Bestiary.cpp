// src/Bestiary.cpp - Bestiary implementation
#include "../include/Bestiary.h"

void Bestiary::add(const BestiaryEntry &e) { entries_.push_back(e); }

const std::vector<BestiaryEntry> &Bestiary::entries() const { return entries_; }

