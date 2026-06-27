#include"logic.h"
#include"tile_manager.h"

Logic::Logic(int width, int height) : width{ width }, height{ height }, map_1 { (size_t)height }, map_2{ (size_t)height }, active_map{ &map_1 }, inactive_map{ &map_2 } {
#ifdef VEC_UNSAFE
	for (int i = 0; i < height; i++) {
		map_1[i] = vector<TileType>(width);
		map_2[i] = vector<TileType>(width);
		for (int j = 0; j < width; j++) {
			map_1[i][j] = TileType::EMPTY;
			map_2[i][j] = TileType::EMPTY;
		}
}
#else
	for (int i = 0; i < height; i++) {
		map_1.at(i) = vector<TileType>(width);
		map_2.at(i) = vector<TileType>(width);
		for (int j = 0; j < width; j++) {
			map_1.at(i).at(j) = tile(empty);
			map_2.at(i).at(j) = tile(empty);
		}
	}
#endif

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

// TODO remove, writen for debugging
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

TileType Logic::get_tile_at(int width, int height) {
#ifdef VEC_UNSAFE
	return in_range(width, height) ? (*active_map)[height][width] : tile(boob);
#else
	return in_range(width, height) ? active_map->at(height).at(width) : tile(boob);
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

bool Logic::in_range(int w, int h) {
	return w >= 0 && h >= 0 && w < width && h < height;
}