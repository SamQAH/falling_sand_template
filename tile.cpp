#include"tile.h"

map<TileType, char> tile_display_char = {
	{TileType::BOOB, '@'},
	{TileType::EMPTY, ' '},
	{TileType::GROUND, '='},
	{TileType::WATER, '.' },
	{TileType::LAVA, 'L'},
	{TileType::SAND, 's'},
	{TileType::STICKY_POWDER, 'S'}
};

map<TileType, string> tile_string = {
	{TileType::BOOB, "Bounds"},
	{TileType::EMPTY, "Empty"},
	{TileType::GROUND, "Ground"},
	{TileType::WATER, "Water"},
	{TileType::LAVA, "Lava"},
	{TileType::SAND, "Sand"},
	{TileType::STICKY_POWDER, "Sticky Powder"}
};

map<string, TileType> string_tile = {
	{"bounds", TileType::BOOB},
	{"empty", TileType::EMPTY},
	{"ground", TileType::GROUND},
	{"water", TileType::WATER},
	{"lava", TileType::LAVA},
	{"sand", TileType::SAND},
	{"sticky powder", TileType::STICKY_POWDER}
};

map<TileType, TileProperties> tile_properties = {
	{TileType::BOOB, { -1,
						{ 0, {} },
						{ true, 1000, {} } }
	},
	{TileType::EMPTY, { -1,
						{ 0, {} },
						{ true, 1000, {} } }
	},
	{TileType::GROUND, { -1,
						{ 1, {} },
						{ true, 10, {} } }
	},
	{TileType::WATER, { 2,
						{ 1, {} },
						{ false, 5, {} } }
	},
	{TileType::LAVA, { 8,
						{ 1, {} },
						{ false, 9, {} } }
	},
	{TileType::SAND, { 1,
						{ 1, {} },
						{ false, 7, {} } }
	},
	{TileType::STICKY_POWDER, { 4,
						{ 1, {} },
						{ false, 7, {} } }
	}
};


char to_display_char(const TileType tp) {
	try {
		return tile_display_char.at(tp);
	}
	catch (int e) {
		return '?';
	}
}

string to_string(const TileType tp) {
	try {
		return tile_string.at(tp);
	}
	catch (int e) {
		return "Unknown";
	}
}

list<Location> TileIter(std::function<TileType(int, int)> get) {
	list<Location> requests;
	TileType curr_tp = get(0, 0);
	if (get(0, 1) != TileType::BOOB) {
		requests.emplace_back( Location{ 0,1,curr_tp } );
	}
	return requests;
}