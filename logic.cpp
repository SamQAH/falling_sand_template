#include"logic.h"
#include"tile_manager.h"

Logic::Logic(int width, int height) : width{ width }, height{ height }, map_1 { (size_t)height }, map_2{ (size_t)height }, active_map{ &map_1 }, inactive_map{ &map_2 } 
{
	TileType empty_tile = tile(empty);
#ifdef VEC_UNSAFE
	for (int i = 0; i < height; i++) {
		map_1[i] = vector<TileType>(width);
		map_2[i] = vector<TileType>(width);
		for (int j = 0; j < width; j++) {
			map_1[i][j] = empty_tile;
			map_2[i][j] = empty_tile;
		}
}
#else
	for (int i = 0; i < height; i++) {
		map_1.at(i) = vector<TileType>(width);
		map_2.at(i) = vector<TileType>(width);
		for (int j = 0; j < width; j++) {
			map_1.at(i).at(j) = empty_tile;
			map_2.at(i).at(j) = empty_tile;
		}
	}
#endif

}

Logic::Logic(const Logic& other) :width{ other.width }, height{ other.height }, map_1{ other.map_1 }, map_2{ other.map_2 }, active_map{ &map_1 }, inactive_map{ &map_2 } 
{

}

Logic& Logic::operator=(const Logic& other)
{
	Logic temp{ other };
	swap(*this, temp);
	return *this;
}

Logic& Logic::operator=(Logic&& other)
{
	swap(*this, other);
	return *this;
}

void Logic::set_get_tile_relative(int width, int height) {
	relative_width = width;
	relative_height = height;
}

TileType Logic::get_tile_at_relative(int width, int height) {
	return get_tile_at(relative_width + width, relative_height + height);
}

vector<int> table = {0, 10, 8, 2, 5, 15, 13, 7, 1, 11, 9, 3, 4, 14, 12, 6};
bool Logic::isCurrentLogicStep(int row, int col) {
	return table.at(step_count % 16) == ((row % 4) * 4 + (col % 4));
}

void Logic::full_step() {
	for (int i = 0; i < num_step_full; i++) {
		step();
	}
}

int Logic::get_width() const
{
	return width;
}

int Logic::get_height() const
{
	return height;
}

// TODO remove, writen for debugging Tile iteration
string compare_locs_list(list<Location>& a, list<Location>& b) {
	ostringstream oss;
	if (a.size() != b.size()) {
		oss << "Size " << a.size() << "; ";
		for (auto& loc : a) {
			oss << loc << ";";
		}
		oss << "\nSize " << b.size() << "; ";
		for (auto& loc : b) {
			oss << loc << ";";
		}
		oss << endl;
		return oss.str();
	}
	for (auto& loc : a) {
		bool found = false;
		for (auto& locb : b) {
			if (loc == locb) {
				found = true;
				break;
			}
		}
		if (!found) {
			for (auto& loc : a) {
				oss << loc << ";";
			}
			oss << endl;
			for (auto& loc : b) {
				oss << loc << ";";
			}
			oss << endl;
			return oss.str();
		}
	}
	return oss.str();
}

void Logic::step() {
	try {
		*inactive_map = *active_map;
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {
				if (!isCurrentLogicStep(row, col)) continue;
				size_t tile_freq = TileManager::at(active_map->at(row).at(col))->get_updateFrequency();
				if (tile_freq == 0 || (step_count / num_step_full) % (tile_freq + 1) != 0) continue;
				set_get_tile_relative(col, row);
				auto get_f = bind(&Logic::get_tile_at_relative, this, placeholders::_1, placeholders::_2);
				list<Location> request_actions = TileManager::at(get_f(0,0))->iterLogic(get_f);
				
				for (auto& loc : request_actions) {
					// cerr << loc.row << loc.col << to_string(loc.tp) << endl;
#ifdef VEC_UNSAFE
					(*inactive_map)[row + loc.row][col + loc.col] = loc.tp;
#else
					inactive_map->at(row + loc.row).at(col + loc.col) = loc.tp;
#endif
				}

			}
		}
		step_count++;
		swap(active_map, inactive_map);
	}
	catch (const std::out_of_range& e) {
		cerr << e.what() << endl;
		cerr << "At Logic::step" << endl;
	}
}

MAPTYPE*& Logic::get_active_map() {
	return active_map;
}

bool Logic::set_tile_at(int width, int height, TileType tp) {
	if (!in_range(width, height)) {
		return false;
	}
#ifdef VEC_UNSAFE
	(*active_map)[height][width] = tp;
#else
	active_map->at(height).at(width) = tp;
#endif
	return true;

}

TileType Logic::get_tile_at(int width, int height) const {
#ifdef VEC_UNSAFE
	return in_range(width, height) ? (*active_map)[height][width] : TILE_SENTINAL
#else
	return in_range(width, height) ? active_map->at(height).at(width) : TILE_SENTINAL;
#endif
}

bool Logic::set_tiles_rect(int width, int height, int wid, int hei, TileType tp) {
	try {
		*inactive_map = *active_map;
		for (int i = height; i < height + hei; i++) {
			vector<TileType>& row = inactive_map->at(i);
			for (int j = width; j < width + wid; j++) {
				row.at(j) = tp;
			}
		}
		swap(active_map, inactive_map);
		return true;
	}
	catch (const std::out_of_range& e) {
		cerr << e.what() << endl;
		cerr << "At Logic::set tiles rect" << endl;
		return false;
	}
}

bool Logic::in_range(int w, int h) const {
	return w >= 0 && h >= 0 && w < width && h < height;
}

bool Logic::save_to_file(const string& filename)
{
	ofstream ofs{ filename };
	if (!(ofs.good())) {
		string error = "logic could not save to: " + filename + "\n";
		throw invalid_argument(error.c_str());
	}
	ofs << width << ' ' << height << endl;
	for (auto& row : *active_map) {
		for (auto& cell : row) {
			ofs << TileManager::at(cell)->get_name() << ' ';
		}
		ofs << endl;
	}
	return true;
}

bool Logic::load_from_file(const string& filename)
{
	ifstream ifs{ filename };
	if (!(ifs.good())) {
		string error = "Logic failed to load from: " + filename + "\n";
		throw invalid_argument(error.c_str());
	}
	int w, h;
	string buffer;
	ifs >> w >> h;
	Logic temp{ w, h };
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			ifs >> buffer;
			temp.set_tile_at(j, i, TileManager::find(buffer));
		}
	}
	*this = temp;
	return true;
}

void swap(Logic& a, Logic& b)
{
	bool a_active1 = a.active_map == &(a.map_1);
	bool b_active1 = b.active_map == &(b.map_1);
	swap(a.width, b.width);
	swap(a.height, b.height);
	swap(a.map_1, b.map_1);
	swap(a.map_2, b.map_2);
	a.active_map = b_active1 ? &a.map_1 : &a.map_2;
	b.active_map = a_active1 ? &b.map_1 : &b.map_2;
}
