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
	LAVA,
	STEAM
};

typedef vector<vector<TileType>> MAPTYPE;

enum DirectionVector {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	ZERO
};
struct MoveLogicProbabilityLayer {
	vector<DirectionVector> choices;
	vector<float> probs;
};
/* physical movement logic
*/
struct TileMovementProperties {
	bool isPositionStable;
	int density;
	list<MoveLogicProbabilityLayer> logicLayers;
};
/* reaction types, indicates which of reagent_one, reagent_two, resultant_one, resultant_two to use
* catalytic A + B -> A + C a type of synthesis
* self-catalytic A + BOOB -> B special
* synthesis A + B -> C + D all can be encoded as this, only thing is the number of requests that are sent
* generative A + EMPTY -> A + B a kind of decomposition
* decompose A + EMPTY -> B + C a kind of synthesis
* desolve A + B -> B + A special case of synthesis
*/
enum ChemReactionType {
	CATALYTIC,
	SELF_CATALYTIC,
	SYNTHESIS,
	GENERATIVE,
	DECOMPOSE,
	ONE_TO_ONE,
	TWO_TO_TWO
};
/*
* probability of reaction as a function of both reactivities
* higher reactivity means higher chance of reaction succeeding
*/
struct ChemReaction {
	ChemReactionType reactionType;
	TileType reagent_one, reagent_two, resultant_one, resultant_two;
};
struct TileChemicalProperties {
	float reactivity;
	map<TileType, vector<ChemReaction>> possible_reactions; // probably faster than vector<ChemReaction>
};
/* flattened summary
updateFrequency
possible_chemical_reactions
positionally_stable
density
move_logic_layers, each layer has its own probabilities
*/
struct TileProperties {
	int updateFrequency; // for logic module to know when to call
	TileChemicalProperties chemProperties;
	TileMovementProperties moveProperties;
};
//not used
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
// not used
class AbstractTile {
public:
	virtual char to_char() = 0;
	virtual string to_string() = 0;
	virtual uint_fast8_t get_update_frequency() = 0;
	virtual list<Location> iterLogic(function<TileType(int, int)> get) = 0;
	virtual ~AbstractTile() = 0;
};


//not used
class BasicTile : public AbstractTile {
	Location iterLogic(function<TileType(int, int)> get);
};

// char to_display_char(tiletype) returns the byte representing the tiletype
char to_display_char(const TileType tp);

// string to_string(tiletype) returns the string representing the tiletype
string to_string(const TileType tp);

/* requests TileIter(getFunction) processes the step for the tile at 0, 0
* chemical sim: processes the TileChemicalProperties
* physics sim: processes the TileMovementProperties
* emotional sim: extra data tied to a few Locations
* 
*/
list<Location> TileIter(function<TileType(int, int)> get);


#endif // !TILE_H

