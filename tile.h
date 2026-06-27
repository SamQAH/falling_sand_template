#ifndef TILE_H
#define TILE_H

#include"config.h"
#include"json.h"

typedef char TILEDATATYPE;
typedef TILEDATATYPE TileType;


typedef vector<vector<TileType>> MAPTYPE;

// bitpacked pair of 2 'shortshort' used as relative coords
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
istream& operator>>(istream& in, DirectionVector& direction);
ostream& operator<<(ostream& out, const DirectionVector& direction);

/* will attempt to move in the direction if roll succeeds
* if going up has a 20% chance and going down has a 50% chance, the probs should be 0.2, 0.7
*/
struct MoveLogicProbabilityLayer {
	vector<DirectionVector> choices;
	vector<float> probs;
};
istream& operator>>(istream& in, MoveLogicProbabilityLayer& move);
ostream& operator<<(ostream& out, const MoveLogicProbabilityLayer& move);
MoveLogicProbabilityLayer operator*(float a, const MoveLogicProbabilityLayer& b);
bool operator==(const MoveLogicProbabilityLayer& a, const MoveLogicProbabilityLayer& b);


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
ostream& operator<<(ostream& out, const ChemReaction& cr);

#ifdef JSON_PARSE_H
ChemReaction json_to_ChemReaction(unique_ptr<JsonObject>& obj);
#endif

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

extern map<string, list<MoveLogicProbabilityLayer>> default_movelogic_options;

struct Location {
	int col;
	int row;
	TileType tp;
};
istream& operator>>(istream& in, Location& loc);
ostream& operator<<(ostream& out, const Location& loc);
bool operator==(const Location& a, const Location& b);


// not used
class AbstractTile {
	friend class TileManager;
protected:
	TILEDATATYPE id; // unique identifier set by TileManager
	char displayChar; // character to be printed in the cli
	string name; // name or description of the tile
	size_t updateFrequency; // every how many should logic call iterLogic, 0 is never
	map<TileType, vector<ChemReaction>> chemicalReactions; // 
	bool isChemicallyStable; // Logic will not call chemIterLogic
	bool isPositionStable; // Logic will not call moveIterLogic
	int density; // determines the settling behaviors of fluids
	list<MoveLogicProbabilityLayer> moveLogicLayers; // determines behavir of moveIterLogic
	virtual list<Location> chemIterLogic(function<TileType(int, int)> get);
	virtual list<Location> moveIterLogic(function<TileType(int, int)> get);
	virtual list<Location> extraIterLogic(function<TileType(int, int)> get);
public:
	AbstractTile();
	bool add_to_chemreaction(ChemReaction& cr);
#ifdef JSON_PARSE_H
	AbstractTile(JsonTree& data);
	static list<MoveLogicProbabilityLayer> json_to_movelogic(const JsonList* data);
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
	virtual string to_string() const;
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
	//virtual string to_string() const;
};



/* requests TileIter(getFunction) processes the step for the tile at 0, 0
* chemical sim: processes the TileChemicalProperties
* physics sim: processes the TileMovementProperties
* emotional sim: extra data tied to a few Locations
* 
*/
list<Location> TileIter(function<TileType(int, int)> get);


#endif // !TILE_H

