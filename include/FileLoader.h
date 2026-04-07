// include/FileLoader.h - helpers to load CSV data
#ifndef ALTERDUNE_FILELOADER_H
#define ALTERDUNE_FILELOADER_H

#include "Item.h"
#include "Monster.h"
#include <string>
#include <vector>

namespace FileLoader {
	bool loadItems(const std::string &path, std::vector<Item> &out);
	bool loadMonsters(const std::string &path, std::vector<std::unique_ptr<Monster>> &out);
}

#endif // ALTERDUNE_FILELOADER_H

