#include"tile.h"
#include<random>
#include"tile_manager.h"

map<string, list<MoveLogicProbabilityLayer>> default_movelogic_options = {
	{"gravity only",
		{
			{{DirectionVector::DOWN}, {1}}
		}
	},
	{"powder",
		{
			{{DirectionVector::DOWN}, {1}},
			MoveLogicProbabilityLayer{ {DirectionVector::Q3, DirectionVector::Q4}, {0.45, 0.45} }
		}
	},
	{"liquid",
		{
			MoveLogicProbabilityLayer{ {DirectionVector::DOWN}, {1} },
			MoveLogicProbabilityLayer{ {DirectionVector::LEFT, DirectionVector::RIGHT}, {0.45, 0.45} }
		}
	},
	{"gas",
		{
			MoveLogicProbabilityLayer{ {DirectionVector::UP, DirectionVector::DOWN, DirectionVector::LEFT, DirectionVector::RIGHT}, {0.3, 0.15, 0.2, 0.2} }
		}
	},
	{"disperse",
		{
			MoveLogicProbabilityLayer{ {DirectionVector::UP, DirectionVector::DOWN, DirectionVector::LEFT, DirectionVector::RIGHT}, {0.2, 0.2, 0.2, 0.2} }
		}
	},
};

istream& operator>>(istream& in, DirectionVector& direction) {
	string buffer = "";
	in >> buffer;
	if (!in) {
		return in;
	}
	if ("up" == buffer) {
		direction = DirectionVector::UP;
	}else if ("down" == buffer) {
		direction = DirectionVector::DOWN;
	}else if ("left" == buffer) {
		direction = DirectionVector::LEFT;
	}else if( "right" == buffer) {
		direction = DirectionVector::RIGHT;
	}else if( "q1" == buffer) {
		direction = DirectionVector::Q1;
	}else if( "q2" == buffer) {
		direction = DirectionVector::Q2;
	}else if( "q3" == buffer) {
		direction = DirectionVector::Q3;
	}else if( "q4" == buffer) {
		direction = DirectionVector::Q4;
	}else if( "N" == buffer) {
		direction = DirectionVector::UP;
	}else if( "S" == buffer) {
		direction = DirectionVector::DOWN;
	}else if( "W" == buffer) {
		direction = DirectionVector::LEFT;
	}else if( "E" == buffer) {
		direction = DirectionVector::RIGHT;
	}else if( "NE" == buffer) {
		direction = DirectionVector::Q1;
	}else if( "NW" == buffer) {
		direction = DirectionVector::Q2;
	}else if( "SW" == buffer) {
		direction = DirectionVector::Q3;
	}
	else if ("SE" == buffer) {
		direction = DirectionVector::Q4;
	}
	else {
		direction = DirectionVector::ZERO;
	}
	return in;
}

ostream& operator<<(ostream& out, const DirectionVector& direction) {
	switch (direction) {
	case DirectionVector::UP:
		out << "up";
		break;
	case DirectionVector::DOWN:
		out << "down";
		break;
	case DirectionVector::LEFT:
		out << "left";
		break;
	case DirectionVector::RIGHT:
		out << "right";
		break;
	case DirectionVector::Q1:
		out << "NE";
		break;
	case DirectionVector::Q2:
		out << "NW";
		break;
	case DirectionVector::Q3:
		out << "SW";
		break;
	case DirectionVector::Q4:
		out << "SE";
		break;
	case DirectionVector::ZERO:
		out << "zero";
		break;
	default:
		out << "[Unknown DirectionVector]";
		break;
	}
	return out;
}

istream& operator>>(istream& in, MoveLogicProbabilityLayer& move) {
	//TODO
	return in;
}

ostream& operator<<(ostream& out, const MoveLogicProbabilityLayer& move) {
	auto miter = move.choices.begin();
	auto piter = move.probs.begin();
	while (miter != move.choices.end() && piter != move.probs.end()) {
		out << *miter << " " << *piter << ",";
		++miter;
		++piter;
	}
	if (miter != move.choices.end() || piter != move.probs.end()) {
		out << "lengths of arrays in move logic layer doesn't match." << endl;
	}
	return out;
}

MoveLogicProbabilityLayer operator*(float a, const MoveLogicProbabilityLayer& b) {
	auto copy = b;
	for (auto& s : copy.probs) {
		s *= a;
	}
	return copy;
}

bool operator==(const MoveLogicProbabilityLayer& a, const MoveLogicProbabilityLayer& b) {
	return false;
}

ostream& operator<<(ostream& out, const ChemReaction& cr) {
	out << "[" << TileManager::at(cr.reagent_one)->get_name();
	out << "+" << TileManager::at(cr.reagent_two)->get_name();
	out << "->" << TileManager::at(cr.resultant_one)->get_name();
	out << "+" << TileManager::at(cr.resultant_two)->get_name();
	out << "|" << cr.probability << "]";
	return out;
}

istream& operator>>(istream& in, Location& loc) {
	int row, col, tp;
	in >> row >> col >> tp;
	if (!in) {
		return in;
	}
	loc.row = row;
	loc.col = col;
	loc.tp = (TileType)tp;
	return in;
}

ostream& operator<<(ostream& out, const Location& loc) {
	out << loc.row << " " << loc.col << " " << (int)loc.tp;
	return out;
}

bool operator==(const Location& a, const Location& b) {
	return a.col == b.col && a.row == b.row && a.tp == b.tp;
}

ChemReaction json_to_ChemReaction(unique_ptr<JsonObject>& obj)
{
	if (obj->get_data_type() != JsonType::TREE) {
		string error = "json_to_ChemReaction expected object of type tree\n" + obj->to_string();
		throw invalid_argument(error.c_str());
	}
	JsonTree* tempobj= (JsonTree*)obj.get();
	if (tempobj->value.find("type") == tempobj->value.end() || tempobj->value.at("type")->get_data_type() != JsonType::STRING) {
		string error = "json_to_ChemReaction missing string field: type\n" + obj->to_string();
		throw invalid_argument(error.c_str());
	}
	if (tempobj->value.find("reaction") == tempobj->value.end() || tempobj->value.at("reaction")->get_data_type() != JsonType::LIST) {
		string error = "json_to_ChemReaction missing list field: reaction\n" + obj->to_string();
		throw invalid_argument(error.c_str());
	}
	if (tempobj->value.find("probability") == tempobj->value.end() || tempobj->value.at("probability")->get_data_type() != JsonType::DOUBLe) {
		string error = "json_to_ChemReaction missing double field: probability\n" + obj->to_string();
		throw invalid_argument(error.c_str());
	}
	string crt_str = tempobj->value.at("type")->to_string();
	ChemReactionType crt = crt_str == "11" ? ChemReactionType::ONE_TO_ONE : ChemReactionType::TWO_TO_TWO; //TODO determine if ChemReactionType is used or not
	auto& reaction_components = ((JsonList*)(tempobj->value.at("reaction").get()))->value;
	float prob = (float)((JsonDouble*)(tempobj->value.at("probability").get()))->value;
	vector<TileType> reaction_elements;
	for (auto& r : reaction_components) {
		TileType tt = TileManager::find(r->to_string());
		reaction_elements.emplace_back(tt);
	}
	if (crt == ChemReactionType::ONE_TO_ONE) {
		if (reaction_elements.size() != 2) {
			string error = "json_to_ChemReaction reaction size error" + obj->to_string();
			throw invalid_argument(error.c_str());
		}
		return ChemReaction{ crt, reaction_elements.at(0), reaction_elements.at(0), reaction_elements.at(1), reaction_elements.at(1), prob };
	}
	else if (crt == ChemReactionType::TWO_TO_TWO) {
		if (reaction_elements.size() != 4) {
			string error = "json_to_ChemReaction reaction size error" + obj->to_string();
			throw invalid_argument(error.c_str());
		}
		return ChemReaction{ crt, reaction_elements.at(0), reaction_elements.at(1), reaction_elements.at(2), reaction_elements.at(3), prob };
	}
	else {
		string error = "json_to_ChemReaction ChemReactionType error" + obj->to_string();
		throw invalid_argument(error.c_str());
	}
	return ChemReaction();
}


list<Location> AbstractTile::chemIterLogic(function<TileType(int, int)> get)
{
	list<Location> requests;
	return requests;
}

list<Location> AbstractTile::moveIterLogic(function<TileType(int, int)> get)
{
	list<Location> requests;
	return requests;
}

list<Location> AbstractTile::extraIterLogic(function<TileType(int, int)> get)
{
	list<Location> requests;
	return requests;
}

AbstractTile::AbstractTile() : id{ 0 }, displayChar{ '?' }, name{ "unknown" }, updateFrequency{ 0 }, chemicalReactions{ }, isPositionStable{ true }, density{ -1 }, moveLogicLayers{ }
{
}

#ifdef JSON_PARSE_H
AbstractTile::AbstractTile(JsonTree& data)
{
	displayChar = data.value.at("char")->to_string().at(0);
	name = data.value.at("name")->to_string();
	updateFrequency = (size_t)(((JsonDouble*)(data.value.at("update frequency").get()))->value);
	//chemicalReactions = json_to_chemreaction((JsonTree*)data.value.at("chemical reactions").get());
	isPositionStable = data.value.at("position stable")->to_string().at(0) == 't';
	isChemicallyStable = data.value.at("chemical stable")->to_string().at(0) == 't';
	density = (int)(((JsonDouble*)(data.value.at("density").get()))->value);
	moveLogicLayers = json_to_movelogic((JsonList*)data.value.at("move logic").get());
}

bool AbstractTile::add_to_chemreaction(ChemReaction& cr) {
	if (cr.reagent_one != id && cr.reagent_two != id) {
		return false;
	}
	ChemReaction temp_cr = cr;
	if (cr.reagent_one != id) {
		swap(temp_cr.reagent_one, temp_cr.reagent_two);
		swap(temp_cr.resultant_one, temp_cr.resultant_two);
	}
	if (cr.reactionType == ChemReactionType::ONE_TO_ONE || cr.reactionType == ChemReactionType::SELF_CATALYTIC) {
		if (chemicalReactions.find(TILE_SENTINAL) == chemicalReactions.end()) {
			chemicalReactions.emplace(TILE_SENTINAL, vector<ChemReaction>());
		}
		chemicalReactions.at(TILE_SENTINAL).emplace_back(temp_cr);
	}
	else {
		if (chemicalReactions.find(temp_cr.reagent_two) == chemicalReactions.end()) {
			chemicalReactions.emplace(temp_cr.reagent_two, vector<ChemReaction>());
		}
		chemicalReactions.at(temp_cr.reagent_two).emplace_back(temp_cr);
	}

	return true;
}

//TODO implement parse
list<MoveLogicProbabilityLayer> AbstractTile::json_to_movelogic(const JsonList* data)
{
	list<MoveLogicProbabilityLayer> moves;
	if (data->value.size() == 0) {
		return moves;
	}
	else if (data->value.front()->get_data_type() == JsonType::STRING) {
		try {
			return default_movelogic_options.at(data->value.front()->to_string());
		}
		catch (const exception& e) {
			cerr << "AbstractTile::json_to_movelogic can't find " << data->value.front()->to_string() << endl;
		}
	}
	cerr << "AbstractTile::json_to_movelogic, not yet implemented." << endl;
	
	return moves;
}

#endif

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

bool& AbstractTile::get_isChemicallyStable()
{
	return isChemicallyStable;
}

int& AbstractTile::get_density()
{
	return density;
}

list<MoveLogicProbabilityLayer>& AbstractTile::get_moveLogicLayers()
{
	return moveLogicLayers;
}

BasicTile::BasicTile(TILEDATATYPE id)
{
	get_id() = id;
}

BasicTile::~BasicTile()
{
}

string AbstractTile::to_string() const
{
	ostringstream oss;
	oss << "Tile name:" << name << endl;
	oss << "display char:" << displayChar << endl;
	oss << "update frequency:" << updateFrequency << endl;
	oss << "chemically stable:" << isChemicallyStable << endl;
	oss << "positionally stable:" << isPositionStable << endl;
	oss << "density:" << density << endl;
	oss << "move logic:\n";
	for (auto& layer : moveLogicLayers) {
		oss << "\t" << layer << endl;
	}
	oss << "chemicalReactions:\n";
	for (const auto& react : chemicalReactions) {
		oss << "\t" << TileManager::at(react.first)->get_name() << endl;
		for (const auto& r : react.second) {
			oss << "\t\t" << r << endl;
		}
	}
	return oss.str();
}

AbstractTile::~AbstractTile() {}

list<Location> AbstractTile::iterLogic(function<TileType(int, int)> get)
{
	list<Location> requests;
	// chemical
	if (!isChemicallyStable) {
		requests.splice(requests.end(), chemIterLogic(get));
	}
	if (!(requests.empty()) && requests.front().col == 0 && requests.front().row == 0 && requests.front().tp != get(0, 0)) {
		return requests;
	}
	// physical
	if (!isPositionStable) {
		requests.splice(requests.end(), moveIterLogic(get));
	}
	if (!(requests.empty()) && requests.front().col == 0 && requests.front().row == 0 && requests.front().tp != get(0, 0)) {
		return requests;
	}
	// emotional
	requests.splice(requests.end(), extraIterLogic(get));

	return requests;
}

list<Location> BasicTile::chemIterLogic(function<TileType(int, int)> get)
{
	list<Location> requests;
	auto& chemRec = get_chemicalReactions();
	if (chemRec.size() == 0) return requests;
	vector<Location> adjacencies = { Location{ 0, -1, get(0, -1) }, Location{ 0, 1, get(0, 1) }, Location{ -1, 0, get(-1, 0) }, Location{ 1, 0, get(1, 0) } };
	for (auto adjIter = adjacencies.begin(); adjIter != adjacencies.end(); ++adjIter) {
		auto iter = chemRec.find(adjIter->tp);
		if (iter == chemRec.end()) continue;
		float random = (float)(rand() % 128) / 128;
		vector<ChemReaction>& crt = iter->second;
		ChemReaction choosenReaction;
		float tempSum = 0;
		bool choosedReact = false;
		for (size_t i = 0; i < crt.size(); i++) {
			tempSum += crt.at(i).probability;
			if (random < tempSum) {
				choosenReaction = crt.at(i);
				choosedReact = true;
				break;
			}
		}
		if (!choosedReact) continue;

		// reaction will now occur
		Location& loc = *adjIter;
		switch (choosenReaction.reactionType)
		{
		case ChemReactionType::TWO_TO_TWO:
			requests.emplace_front(Location{ 0, 0, choosenReaction.resultant_one });
			requests.emplace_back(Location{ loc.col, loc.row, choosenReaction.resultant_two });
			loc.tp = choosenReaction.resultant_two;
			break;
		case ChemReactionType::CATALYTIC:
			requests.emplace_back(Location{ loc.col, loc.row, choosenReaction.resultant_two });
			loc.tp = choosenReaction.resultant_two;
			break;
		case ChemReactionType::SYNTHESIS:
			requests.emplace_front(Location{ 0, 0, choosenReaction.resultant_one });
			requests.emplace_back(Location{ loc.col, loc.row, choosenReaction.resultant_two });
			loc.tp = choosenReaction.resultant_two;
			break;
		case ChemReactionType::GENERATIVE:
			requests.emplace_back(Location{ loc.col, loc.row, choosenReaction.resultant_two });
			loc.tp = choosenReaction.resultant_two;
			break;
		case ChemReactionType::DECOMPOSE:
			requests.emplace_front(Location{ 0, 0, choosenReaction.resultant_one });
			requests.emplace_back(Location{ loc.col, loc.row, choosenReaction.resultant_two });
			loc.tp = choosenReaction.resultant_two;
			break;
		default:
			break;
		}
	}
	// self reactions
	for (auto iter = chemRec.find(TILE_SENTINAL); iter != chemRec.end(); iter = chemRec.end()) {
		// roll to determine if self reaction will occur
		vector<ChemReaction>& crt = iter->second;
		float random = (float)(rand() % 128) / 128;
		ChemReaction choosenReaction;
		float tempSum = 0;
		bool choosedReact = false;
		for (size_t i = 0; i < crt.size(); i++) {
			tempSum += crt.at(i).probability;
			if (random < tempSum) {
				choosenReaction = crt.at(i);
				choosedReact = true;
				break;
			}
		}
		if (!choosedReact) break;
		// self reaction will now occur
		switch (choosenReaction.reactionType)
		{
		case ChemReactionType::ONE_TO_ONE:
			requests.emplace_front(Location{ 0, 0, choosenReaction.resultant_one });
			break;
		case ChemReactionType::SELF_CATALYTIC:
			requests.emplace_front(Location{ 0, 0, choosenReaction.resultant_one });
			break;
		default:
			break;
		}
	}
	return requests;
}

list<Location> BasicTile::moveIterLogic(function<TileType(int, int)> get)
{
	list<Location> requests;
	list<MoveLogicProbabilityLayer>& mvLayers = get_moveLogicLayers();
	for (auto& tmp : mvLayers) {
		// roll which direction to try to move
		float random = (float)(rand() % 128) / 128;
		size_t choice_index = 0;
		float prob_sum = 0;
		for (; choice_index < tmp.probs.size(); choice_index++) {
			prob_sum += tmp.probs.at(choice_index);
			if (random < prob_sum) break;
		}
		if (choice_index == tmp.probs.size()) break;
		// check if tile is able to move there
		DirectionVector vec = tmp.choices.at(choice_index);
		int vec_col = (int)(((uint8_t)vec) >> 4) - 1;
		int vec_row = (int)(((uint8_t)vec) % 16) - 1;
		TileType other_tile = get(vec_col, vec_row);
		auto other_tile_ptr = TileManager::at(other_tile);
		if (other_tile_ptr->get_id() == 0 || other_tile_ptr->get_id() == get_id()) continue;
		// additional roll for density
		int momentum = abs(get_density());
		int inertia = abs(other_tile_ptr->get_density());
		if (momentum < inertia) continue;
		
		requests.emplace_back(Location{ 0, 0, other_tile });
		requests.emplace_back(Location{ vec_col, vec_row, get_id() });
		break;

	}
	return requests;
}
