#include"logic.h"

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
			map_1.at(i).at(j) = TileType::EMPTY;
			map_2.at(i).at(j) = TileType::EMPTY;
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

/*
use only swap tiles?
update 9 times 3 by 3?
*/
void Logic::step() {
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			set_get_tile_relative(col, row);
			Location loc = TileIter(bind(&Logic::get_tile_at_relative, this, placeholders::_1, placeholders::_2));
			// cerr << loc.row << loc.col << to_string(loc.tp) << endl;
#ifdef VEC_UNSAFE
			(*inactive_map)[row + loc.row][col + loc.col] = loc.tp;
#else
			inactive_map->at(row + loc.row).at(col + loc.col) = loc.tp;
#endif
		}
	}
	swap(active_map, inactive_map);
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
	return in_range(width, height) ? (*active_map)[height][width] : TileType::BOOB;
#else
	return in_range(width, height) ? active_map->at(height).at(width) : TileType::BOOB;
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
	catch (int e) {
		return false;
	}
}

bool Logic::in_range(int w, int h) {
	return w >= 0 && h >= 0 && w < width && h < height;
}