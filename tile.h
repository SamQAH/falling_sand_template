#ifndef TILE_H
#define TILE_H

#include"config.h"

enum TileType
{
	BOOB, // use as sentinal value
	EMPTY,
	GROUND,
	SAND,
	STICKY_POWDER,
	WATER,
	LAVA
};

typedef vector<vector<TileType>> MAPTYPE;

/* physical movement logic
*/
struct TileMovementProperties {
	bool isPositionStable;
	int density;
	list<vector<float>> probabilities;
};
/* reaction types, indicates which of reagent_one, reagent_two, resultant_one, resultant_two to use
* catalytic A + B -> A + C
* self-catalytic A -> B
* synthesis A + B -> C
* generative A -> A + B
* decompose A -> B + C
* desolve A + B -> B + A
*/
enum ChemReactionType {
	CATALYTIC,
	SELF_CATALYTIC,
	SYNTHESIS,
	GENERATIVE,
	DECOMPOSE,
	DESOLVE
};
/*
* probability of reaction as a function of both reactivities
* higher reactivity means attempting a reaction more often
*/
struct ChemReaction {
	ChemReactionType reactionType;
	TileType reagent_one, reagent_two, resultant_one, resultant_two;
};
struct TileChemicalProperties {
	float reactivity;
	map<TileType, ChemReaction> possible_reactions; // probably faster than vector<ChemReaction>
};
struct TileProperties {
	int updateFrequency; // for logic module to know when to call
	TileChemicalProperties chemProperties;
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
extern map<TileType, TileProperties> tile_properties;

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
physics sim: swaps only, must be evaluation order independent
chemical sim: consistancy matters less
emotional sim: extra data tied to a few Locations
*/
char to_display_char(const TileType tp);

string to_string(const TileType tp);

list<Location> TileIter(function<TileType(int, int)> get);


#endif // !TILE_H

