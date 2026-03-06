#ifndef TILE_H
#define TILE_H

#include<string>
#include<functional>

enum TileType
{
	BOOB,
	EMPTY,
	GROUND,
	SAND,
	STICKY_POWDER,
	WATER,
	LAVA
};

struct Location {
	int col;
	int row;
	TileType tp;
};

class AbstractTile {
public:
	virtual Location iterLogic(std::function<TileType(int, int)> get);
};

class BasicTile : public AbstractTile {
	Location iterLogic(std::function<TileType(int, int)> get);
};
/*
tags/fields
update frequency
density

*/
char to_display_char(const TileType tp);

std::string to_string(const TileType tp);

Location TileIter(std::function<TileType(int, int)> get);


#endif // !TILE_H

