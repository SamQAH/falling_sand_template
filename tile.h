#ifndef TILE_H
#define TILE_H

#include"config.h"

typedef char TILEDATATYPE;

enum TileType : TILEDATATYPE
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
	float reactivity;
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
	friend class TileManager;
private:
	TILEDATATYPE id;
	char displayChar;
	string name;
	size_t updateFrequency;
	map<TileType, vector<ChemReaction>> chemicalReactions;
	bool isPositionStable;
	int density;
	list<MoveLogicProbabilityLayer> moveLogicLayers;
	virtual list<Location> chemIterLogic(function<TileType(int, int)> get);
	virtual list<Location> moveIterLogic(function<TileType(int, int)> get);
	virtual list<Location> extraIterLogic(function<TileType(int, int)> get);
public:
	AbstractTile();
	TILEDATATYPE& get_id();
	char& get_displayChar();
	string& get_name();
	size_t& get_updateFrequency();
	map<TileType, vector<ChemReaction>>& get_chemicalReactions();
	bool& get_isPositionStable();
	int& get_density();
	virtual list<Location> iterLogic(function<TileType(int, int)> get);
	virtual ~AbstractTile() = 0;
};

// no chemical reactions, positionally stable
class BoringTile : public AbstractTile {
public:
	BoringTile(TILEDATATYPE id);
	~BoringTile();
};

// TileManager, manages tile information, must use this class to access
class TileManager {
private:
	size_t first_free;
	vector<shared_ptr<AbstractTile>> tiles;

public:
	TileManager();
	~TileManager() = default;
	shared_ptr<AbstractTile> at(TILEDATATYPE key);
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

