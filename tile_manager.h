#ifndef TILE_MANAGER_H
#define TILE_MANAGER_H

#include"config.h"
#include"json.h"
#include"tile.h"

#define tile(x) TileManager::find(#x)

// TileManager, manages tile information, must use this class to access
class TileManager {
private:
	static size_t first_free; // stores the first empty slot in tiles, all slots before are assumed to be filled
	static vector<shared_ptr<AbstractTile>> tiles; // array of length 2^sizeof(TILEDATATYPE)

public:
	static shared_ptr<AbstractTile> at(TILEDATATYPE key);
	static bool add(shared_ptr<AbstractTile> tile); // adds tile to tiles.at(first_free)
#ifdef JSON_PARSE_H
	static bool add(const string& filename);
	static bool add_reactions(const string& filename);
#endif
	static TILEDATATYPE find(const string& name);
	static TILEDATATYPE find(char chr);
	static string to_string();
};

#endif