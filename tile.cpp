#include"tile.h"

map<TileType, char> tile_display_char = {
	{TileType::BOOB, '@'},
	{TileType::VOID, ' '},
	{TileType::GROUND, '='},
	{TileType::WATER, '.' },
	{TileType::LAVA, 'L'},
	{TileType::SAND, 's'},
	{TileType::STICKY_POWDER, 'S'}
};

map<TileType, string> tile_string = {
	{TileType::BOOB, "Bounds"},
	{TileType::VOID, "Void"},
	{TileType::GROUND, "Ground"},
	{TileType::WATER, "Water"},
	{TileType::LAVA, "Lava"},
	{TileType::SAND, "Sand"},
	{TileType::STICKY_POWDER, "Sticky Powder"}
};

map<string, TileType> string_tile = {
	{"bounds", TileType::BOOB},
	{"void", TileType::VOID},
	{"ground", TileType::GROUND},
	{"water", TileType::WATER},
	{"lava", TileType::LAVA},
	{"sand", TileType::SAND},
	{"sticky powder", TileType::STICKY_POWDER}
};

 


char to_display_char(const TileType tp) {
	return tile_display_char.at(tp);
}

std::string to_string(const TileType tp) {
	return tile_string.at(tp);
}

Location TileIter(std::function<TileType(int, int)> get) {
	TileType curr_tp = get(0, 0);
	if (get(0, 1) != TileType::BOOB) {
		return Location{ 0,1,curr_tp };
	}
	return Location{ 0,0,curr_tp };
}