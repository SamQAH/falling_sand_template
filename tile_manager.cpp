#include"tile_manager.h"

size_t TileManager::first_free = 0;
vector<shared_ptr<AbstractTile>> TileManager::tiles{ 2 ^ (8 * sizeof(TILEDATATYPE)) };

//TileManager::tiles.at(0) = make_shared<BoringTile>(0);

shared_ptr<AbstractTile> TileManager::at(TILEDATATYPE key)
{
#ifdef VEC_UNSAFE
	shared_ptr<AbstractTile> tile = tiles[key];
	return tile ? tile : tiles[0];
#else
	shared_ptr<AbstractTile> tile = tiles.at(key);
	return tile ? tile : tiles.at(0);
#endif
}

bool TileManager::add(shared_ptr<AbstractTile> tile)
{
	if (!tile) {
		return false;
	}
	tile->get_id() = first_free;
	tiles.at(first_free++) = tile;
	return true;
}

#ifdef JSON_PARSE_H
bool TileManager::add(const string& filename)
{
	unique_ptr<JsonObject> smt_decon = file_to_json(filename);
	JsonObject* tile_properties = smt_decon.get();
	if (!tile_properties) {
		return false;
	}
	if (tile_properties->get_data_type() != JsonType::LIST) {
		cerr << "json format incorrect," << (size_t)(tile_properties->get_data_type()) << " expected list: " << filename << endl;
		return false;
	}
#ifdef VERBOSE
	cerr << tile_properties->to_string() << endl;
#endif
	for (auto& tile : ((JsonList*)tile_properties)->value) {
#ifdef VERBOSE
		cerr << "New substance added" << tile->to_string() << endl;
#endif
		try {
			if (tile->get_data_type() != JsonType::TREE) {
				cerr << "json format incorrect, expected map/dictionary: " << filename << endl;
				continue;
			}
			auto& tile_p = ((JsonTree*)tile.get())->value;
			shared_ptr<AbstractTile> temp_tile = make_shared<BasicTile>(0);
			temp_tile->get_displayChar() = tile_p.at("char")->to_string().at(0);
			temp_tile->get_name() = tile_p.at("name")->to_string();
			temp_tile->get_updateFrequency() = (size_t)(((JsonDouble*)(tile_p.at("update frequency").get()))->value);
			//temp_tile->get_chemicalReactions() = json_to_chemreaction((JsonTree*)tile_p.at("chemical reactions"));
			temp_tile->get_isPositionStable() = tile_p.at("position stable")->to_string().at(0) == 't';
			temp_tile->get_isChemicallyStable() = tile_p.at("chemical stable")->to_string().at(0) == 't';
			temp_tile->get_density() = (int)(((JsonDouble*)(tile_p.at("density").get()))->value);
			temp_tile->get_moveLogicLayers() = AbstractTile::json_to_movelogic((JsonList*)tile_p.at("move logic").get());
			add(temp_tile);
		}
		catch (const exception& e) {
			cerr << "tile property json format error, " << e.what() << endl;
		}
	}
	return true;
}
//TODO fix
bool TileManager::add_reactions(const string& filename)
{
	unique_ptr<JsonObject> smt_decon = file_to_json(filename);
	JsonObject* reactions = smt_decon.get();
	if (!reactions) {
		return false;
	}
	if (reactions->get_data_type() != JsonType::LIST) {
		cerr << "json format incorrect," << (size_t)(reactions->get_data_type()) << " expected list: " << filename << endl;
		return false;
	}
#ifdef VERBOSE
	cerr << reactions->to_string() << endl;
#endif
	for (auto& react : ((JsonList*)reactions)->value) {
		ChemReaction cr = json_to_ChemReaction(react);
#ifdef VERBOSE
		cerr << "adding " << cr << endl;
#endif
		for (size_t i = 1; i < TileManager::first_free; i++) {
			TileManager::at(i)->add_to_chemreaction(cr);
		}
	}
	return true;
}
#endif

TILEDATATYPE TileManager::find(const string& name)
{
	for (size_t i = 0; i < tiles.size(); i++) {
		if (tiles.at(i)->get_name() == name) {
			return (TILEDATATYPE)i;
		}
	}
	return (TILEDATATYPE)0;
}

TILEDATATYPE TileManager::find(char chr)
{
	for (size_t i = 0; i < tiles.size(); i++) {
		if (tiles.at(i)->get_displayChar() == chr) {
			return (TILEDATATYPE)i;
		}
	}
	return (TILEDATATYPE)0;
}

string TileManager::to_string()
{
	ostringstream oss;
	for (size_t i = 0; i < first_free; i++) {
		oss << tiles.at(i)->to_string() << endl;
	}
	oss << "Total of " << first_free << " tiles being managed." << endl;
	return oss.str();
}
