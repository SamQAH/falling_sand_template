#include"tile.h"
#include<random>

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
	TileProperties p = tile_properties.at(curr_tp);
	TileChemicalProperties cp = p.chemProperties;
	bool currtpChanged = false;
	TileMovementProperties mp = p.moveProperties;
	vector<Location> adjacencies = { Location{ 0, -1, get(0, -1) }, Location{ 0, 1, get(0, 1) }, Location{ -1, 0, get(-1, 0) }, Location{ 1, 0, get(1, 0) } };
	// chemical reactions with other tiles
	for (auto adjIter = adjacencies.begin(); adjIter != adjacencies.end() && cp.reactivity != 0; ++adjIter) {
		Location& loc = *adjIter;
		auto iter = cp.possible_reactions.find(loc.tp);
		float random = (rand() % 128) / 128;
		if (iter == cp.possible_reactions.end() || cp.reactivity * tile_properties.at(loc.tp).chemProperties.reactivity > random) continue;
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
	{
		auto iter = cp.possible_reactions.find(TileType::BOOB);
		if (iter != cp.possible_reactions.end()) {
			const vector<ChemReaction>& crt = iter->second;
			const ChemReaction& choosenReaction = crt.at(rand() % crt.size());
			float random = (rand() % 128) / 128;
			if (cp.reactivity > random) break;
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
	for (auto& moveIter = mp.logicLayers.begin(); moveIter != mp.logicLayers.end() && !(mp.isPositionStable); ++moveIter) {
		MoveLogicProbabilityLayer& tmp = *moveIter;
		float random = (rand() % 128) / 128;
		int choice_index = 0;
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