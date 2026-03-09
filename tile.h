#ifndef TILE_H
#define TILE_H

#include"config.h"

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

typedef vector<vector<TileType>> MAPTYPE;

struct TileMovementProperties {
	bool isPositionStable;
	bool isCanFall;
	bool isCanFallUp;
	bool isCanPyramid;
	bool isCanPyramidUp;
	bool isCanSpread;
	bool isCanExpand;
};
struct TileProperties {
	int updateFrequency;
	int density;
    int reactivity;
	bool isChemStable;
	TileMovementProperties moveProperties;
};

bool canFall(function<TileType(int, int)>);
bool canFallUp(function<TileType(int, int)>);
vector<int> canPyramid(function<TileType(int, int)>);
vector<int> canPyramidUp(function<TileType(int, int)>);
vector<int> canSpread(function<TileType(int, int)>);
vector<int> canExpand(function<TileType(int, int)>);

extern map<TileType, char> tile_display_char;
extern map<TileType, string> tile_string;
extern map<string, TileType> string_tile;

struct Location {
	int col;
	int row;
	TileType tp;
};

class AbstractTile {
public:
	virtual Location iterLogic(function<TileType(int, int)> get);
};

class BasicTile : public AbstractTile {
	Location iterLogic(function<TileType(int, int)> get);
};
/*
* TODO just write, refactor later

*/
char to_display_char(const TileType tp);

string to_string(const TileType tp);

Location TileIter(function<TileType(int, int)> get);


#endif // !TILE_H

