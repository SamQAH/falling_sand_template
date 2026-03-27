#ifndef TILE_H
#define TILE_H

#include"config.h"
#include"json.h"

typedef char TILEDATATYPE;

enum TileType : TILEDATATYPE
{
	BOOB, // use as sentinal value
	EMPTY,
	GROUND,
	WATER,
	STEAM,
	LAVA,
	SAND,
	STICKY_POWDER
};

typedef vector<vector<TileType>> MAPTYPE;

// bitpacked pair of 2 'shortshort' used as relatice coords
enum DirectionVector : uint8_t {
	UP = (1 << 4) + 0,
	DOWN = (1 << 4) + 2,
	LEFT = (0 << 4) + 1,
	RIGHT = (2 << 4) + 1,
	Q1 = (2 << 4) + 0,
	Q2 = (0 << 4) + 0,
	Q3 = (0 << 4) + 2,
	Q4 = (2 << 4) + 2,
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
	float probability;
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
	bool isChemicallyStable;
	bool isPositionStable;
	int density;
	list<MoveLogicProbabilityLayer> moveLogicLayers;
	virtual list<Location> chemIterLogic(function<TileType(int, int)> get);
	virtual list<Location> moveIterLogic(function<TileType(int, int)> get);
	virtual list<Location> extraIterLogic(function<TileType(int, int)> get);
public:
	AbstractTile();
#ifdef JSON_PARSE_H
	AbstractTile(JsonTree& data);
#endif
	virtual ~AbstractTile() = 0;
	TILEDATATYPE& get_id();
	char& get_displayChar();
	string& get_name();
	size_t& get_updateFrequency();
	map<TileType, vector<ChemReaction>>& get_chemicalReactions();
	bool& get_isPositionStable();
	bool& get_isChemicallyStable();
	int& get_density();
	list<MoveLogicProbabilityLayer>& get_moveLogicLayers();
	virtual list<Location> iterLogic(function<TileType(int, int)> get);
};

// no chemical reactions, positionally stable
class BasicTile : public AbstractTile {
private:
	virtual list<Location> chemIterLogic(function<TileType(int, int)> get);
	virtual list<Location> moveIterLogic(function<TileType(int, int)> get);

public:
	BasicTile(TILEDATATYPE id);
	~BasicTile();
	//virtual list<Location> iterLogic(function<TileType(int, int)> get);
};

// TileManager, manages tile information, must use this class to access
class TileManager {
private:
	static size_t first_free;
	static vector<shared_ptr<AbstractTile>> tiles;

public:
	static shared_ptr<AbstractTile> at(TILEDATATYPE key);
	static bool add(shared_ptr<AbstractTile> tile);
	static bool add(list<shared_ptr<AbstractTile>>& tile);
#ifdef JSON_PARSE_H
	static bool add(const string& filename);
#endif
	static TILEDATATYPE find(const string& name);
	static TILEDATATYPE find(char chr);
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

