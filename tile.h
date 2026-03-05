#ifndef TILE_H
#define TILE_H

#include <functional>

enum TileType
{
	BOOB,
	VOID,
	GROUND,
	SAND,
	STICKY_POWDER,
	WATER,
	LAVA
};

struct Location {
	int row;
	int col;
	TileType tp;
};

char to_display_char(TileType tp);

Location TileIter(std::function<TileType(int, int)> get);


#endif // !TILE_H

