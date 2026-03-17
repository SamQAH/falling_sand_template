#include"tile.h"
#include<random>

map<TileType, char> tile_display_char = {
	{TileType::BOOB, '@'},
	{TileType::EMPTY, ' '},
	{TileType::GROUND, '='},
	{TileType::WATER, '.' },
	{TileType::STEAM, '\'' },
	{TileType::LAVA, 'L'},
	{TileType::SAND, 's'},
	{TileType::STICKY_POWDER, 'S'}
};

map<TileType, string> tile_string = {
	{TileType::BOOB, "Bounds"},
	{TileType::EMPTY, "Empty"},
	{TileType::GROUND, "Ground"},
	{TileType::WATER, "Water"},
	{TileType::STEAM, "Steam"},
	{TileType::LAVA, "Lava"},
	{TileType::SAND, "Sand"},
	{TileType::STICKY_POWDER, "Sticky Powder"}
};

map<string, TileType> string_tile = {
	{"bounds", TileType::BOOB},
	{"empty", TileType::EMPTY},
	{"ground", TileType::GROUND},
	{"water", TileType::WATER},
	{"steam", TileType::STEAM},
	{"lava", TileType::LAVA},
	{"sand", TileType::SAND},
	{"sticky powder", TileType::STICKY_POWDER}
};

/* reactions and movement
{ChemReactionType::TWO_TO_TWO, TileType::LAVA, TileType::WATER, TileType::GROUND, TileType::STEAM}
{ChemReactionType::ONE_TO_ONE, TileType::STEAM, TileType::BOOB, TileType::WATER, TileType::BOOB}
{ChemReactionType::TWO_TO_TWO, TileType::LAVA, TileType::GROUND, TileType::LAVA, TileType::LAVA}

{ {DirectionVector::DOWN}, {1} }
{ {DirectionVector::LEFT, DirectionVector::RIGHT}, {0.5, 1} }
{ {DirectionVector::UP, DirectionVector::DOWN, DirectionVector::LEFT, DirectionVector::RIGHT}, {0.2, 0.4, 0.6, 0.8} }
*/
map<TileType, TileProperties> tile_properties = {
	{TileType::BOOB, { -1,
						{ 0, {} },
						{ true, 1000, {} } }
	},
	{TileType::EMPTY, { -1,
						{ 0, {} },
						{ true, 0, {} } }
	},
	{TileType::GROUND, { -1,
						{ 0.05, {} },
						{ true, 10, {} } }
	},
	{TileType::WATER, { 2,
						{ 0.3, {} },
						{ false, 5, { MoveLogicProbabilityLayer{ {DirectionVector::DOWN}, {1} }, 
									  MoveLogicProbabilityLayer{ {DirectionVector::LEFT, DirectionVector::RIGHT}, {0.5, 1} }, 
									  MoveLogicProbabilityLayer{ {DirectionVector::LEFT, DirectionVector::RIGHT}, {0.5, 1} } } } }
	},
	{TileType::STEAM, { 2,
						{ 0.01, { {TileType::BOOB, { ChemReaction{ChemReactionType::ONE_TO_ONE, TileType::STEAM, TileType::BOOB, TileType::WATER, TileType::BOOB} } } } },
						{ false, 5, { MoveLogicProbabilityLayer{ {DirectionVector::UP, DirectionVector::DOWN, DirectionVector::LEFT, DirectionVector::RIGHT}, {0.2, 0.4, 0.6, 0.8} } } } }
	},
	{TileType::LAVA, { 4,
						{ 0.9, { {TileType::WATER, { ChemReaction{ChemReactionType::TWO_TO_TWO, TileType::LAVA, TileType::WATER, TileType::GROUND, TileType::STEAM} } },
								{TileType::GROUND, { ChemReaction{ChemReactionType::TWO_TO_TWO, TileType::LAVA, TileType::GROUND, TileType::LAVA, TileType::LAVA} } } } },
						{ false, 9, { MoveLogicProbabilityLayer{ {DirectionVector::DOWN}, {1} }, 
									  MoveLogicProbabilityLayer{ {DirectionVector::LEFT, DirectionVector::RIGHT}, {0.5, 1} }, 
									  MoveLogicProbabilityLayer{ {DirectionVector::LEFT, DirectionVector::RIGHT}, {0.5, 1} } } } }
	},
	{TileType::SAND, { 1,
						{ 0.1, {} },
						{ false, 7, { { {DirectionVector::DOWN}, {1} }, { {DirectionVector::LEFT, DirectionVector::RIGHT}, {0.5, 1} } } } }
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
	TileProperties p = tile_properties.at(curr_tp);
	TileChemicalProperties cp = p.chemProperties;
	bool currtpChanged = false;
	TileMovementProperties mp = p.moveProperties;
	vector<Location> adjacencies = { Location{ 0, -1, get(0, -1) }, Location{ 0, 1, get(0, 1) }, Location{ -1, 0, get(-1, 0) }, Location{ 1, 0, get(1, 0) } };
	// chemical reactions with other tiles
	for (auto adjIter = adjacencies.begin(); adjIter != adjacencies.end() && cp.reactivity != 0; ++adjIter) {
		Location& loc = *adjIter;
		auto iter = cp.possible_reactions.find(loc.tp);
		float random = (float)(rand() % 128) / 128;
		if (iter == cp.possible_reactions.end() || cp.reactivity * tile_properties.at(loc.tp).chemProperties.reactivity < random) continue;
		// reaction will now occur
		const vector<ChemReaction>& crt = iter->second;
		const ChemReaction& choosenReaction = crt.at(rand() % crt.size());
		switch (choosenReaction.reactionType)
		{
		case ChemReactionType::TWO_TO_TWO:
			requests.emplace_back(Location{ 0, 0, choosenReaction.resultant_one });
			requests.emplace_back(Location{ loc.col, loc.row, choosenReaction.resultant_two });
			loc.tp = choosenReaction.resultant_two;
			currtpChanged = choosenReaction.resultant_one != curr_tp;
			break;
		case ChemReactionType::CATALYTIC:
			requests.emplace_back(Location{loc.col, loc.row, choosenReaction.resultant_two});
			loc.tp = choosenReaction.resultant_two;
			break;
		case ChemReactionType::SYNTHESIS:
			requests.emplace_back(Location{ 0, 0, choosenReaction.resultant_one });
			requests.emplace_back(Location{ loc.col, loc.row, choosenReaction.resultant_two });
			loc.tp = choosenReaction.resultant_two;
			currtpChanged = choosenReaction.resultant_one != curr_tp;
			break;
		case ChemReactionType::GENERATIVE:
			requests.emplace_back(Location{ loc.col, loc.row, choosenReaction.resultant_two });
			loc.tp = choosenReaction.resultant_two;
			break;
		case ChemReactionType::DECOMPOSE:
			requests.emplace_back(Location{ 0, 0, choosenReaction.resultant_one });
			requests.emplace_back(Location{ loc.col, loc.row, choosenReaction.resultant_two });
			loc.tp = choosenReaction.resultant_two;
			currtpChanged = choosenReaction.resultant_one != curr_tp;
			break;
		default:
			break;
		}
	}
	// self reactions
	for (int i = 0; i < 1; i++) {
		auto iter = cp.possible_reactions.find(TileType::BOOB);
		if (iter != cp.possible_reactions.end()) {
			const vector<ChemReaction>& crt = iter->second;
			const ChemReaction& choosenReaction = crt.at(rand() % crt.size());
			float random = (float)(rand() % 128) / 128;
			if (cp.reactivity < random) break;
			switch (choosenReaction.reactionType)
			{
			case ChemReactionType::ONE_TO_ONE:
				requests.emplace_back(Location{ 0, 0, choosenReaction.resultant_one });
				currtpChanged = choosenReaction.resultant_one != curr_tp;
				break;
			case ChemReactionType::SELF_CATALYTIC:
				requests.emplace_back(Location{ 0, 0, choosenReaction.resultant_one });
				currtpChanged = choosenReaction.resultant_one != curr_tp;
				break;
			default:
				break;
			}
		}
	}
	if (currtpChanged) return requests;
	
	// physical
	for (auto moveIter = mp.logicLayers.begin(); moveIter != mp.logicLayers.end() && !(mp.isPositionStable); ++moveIter) {
		MoveLogicProbabilityLayer& tmp = *moveIter;
		float random = (float)(rand() % 128) / 128;
		size_t choice_index = 0;
		for (; choice_index < tmp.probs.size(); choice_index++) {
			if (tmp.probs.at(choice_index) > random) break;
		}
		if (choice_index == tmp.probs.size()) break;
		Location otherLocCopy = adjacencies.at((int)(tmp.choices.at(choice_index)));
		if (tile_properties.at(otherLocCopy.tp).moveProperties.density > mp.density) continue;
		requests.emplace_back(Location{ 0, 0, otherLocCopy.tp });
		otherLocCopy.tp = curr_tp;
		requests.emplace_back(otherLocCopy);
		break;

	}
	// emotional

	return requests;
}

list<Location> AbstractTile::chemIterLogic(function<TileType(int, int)> get)
{
	return list<Location>();
}

list<Location> AbstractTile::moveIterLogic(function<TileType(int, int)> get)
{
	return list<Location>();
}

list<Location> AbstractTile::extraIterLogic(function<TileType(int, int)> get)
{
	return list<Location>();
}

AbstractTile::AbstractTile() : id{ 0 }, displayChar{ '?' }, name{ "unknown" }, updateFrequency{ 0 }, chemicalReactions{ }, isPositionStable{ true }, density{ 2147483647 }, moveLogicLayers{ }
{
}

TILEDATATYPE& AbstractTile::get_id()
{
	return id;
}

char& AbstractTile::get_displayChar()
{
	return displayChar;
}

string& AbstractTile::get_name()
{
	return name;
}

size_t& AbstractTile::get_updateFrequency()
{
	return updateFrequency;
}

map<TileType, vector<ChemReaction>>& AbstractTile::get_chemicalReactions()
{
	return chemicalReactions;
}

bool& AbstractTile::get_isPositionStable()
{
	return isPositionStable;
}

int& AbstractTile::get_density()
{
	return density;
}

list<Location> AbstractTile::iterLogic(function<TileType(int, int)> get)
{
	list<Location> requests;
	requests.splice(requests.end(), chemIterLogic(get));
	if (requests.front().col == 0 && requests.front().row == 0) {
		return requests;
	}
	requests.splice(requests.end(), moveIterLogic(get));

	if (requests.front().col == 0 && requests.front().row == 0) {
		return requests;
	}
	requests.splice(requests.end(), extraIterLogic(get));

	return requests;
}

AbstractTile::~AbstractTile()
{
}

BoringTile::BoringTile(TILEDATATYPE id)
{
	get_id() = id;
}

BoringTile::~BoringTile()
{
}

TileManager::TileManager() : first_free{ 0 }, tiles{ 2 ^ (8 * sizeof(TILEDATATYPE)) }
{
	auto temp = make_shared<BoringTile>(0);
	tiles.at(0) = temp;
}

shared_ptr<AbstractTile> TileManager::at(TILEDATATYPE key)
{
	return tiles.at(key) ? tiles.at(key) : tiles.at(0);
}
