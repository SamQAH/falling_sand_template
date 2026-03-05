#include"logic.h"

Logic::Logic(int width, int height) : width{ width }, height{ height }, map_1 { (size_t)height }, map_2{ (size_t)height }, active_map{ &map_1 }, inactive_map{ &map_2 } {
	for (int i = 0; i < height; i++) {
		map_1.at(i) = vector<TileType>(width);
		map_2.at(i) = vector<TileType>(width);
		for (int j = 0; j < width; j++) {
			map_1.at(i).at(j) = TileType::VOID;
			map_2.at(i).at(j) = TileType::VOID;
		}
	}
}

void Logic::set_get_tile_relative(int width, int height) {
	relative_width = width;
	relative_height = height;
}

TileType Logic::get_tile_at_relative(int width, int height) {
	return get_tile_at(relative_width + width, relative_height + height);
}

void Logic::step() {
	cout << "enter step"<<endl;
	for (int row = 0; row < height; row++) {
		cout << "enter step height" << endl;
		for (int col = 0; col < width; col++) {
			cout << "enter step width" << endl;
			set_get_tile_relative(col, row);
			Location loc = TileIter(bind(&Logic::get_tile_at_relative, this, placeholders::_1, placeholders::_2));
			cout << loc.row << loc.col << to_display_char(loc.tp) << endl;
			inactive_map->at(row + loc.row).at(col + loc.col) = loc.tp;
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
	active_map->at(height).at(width) = tp;
	return true;

}

TileType Logic::get_tile_at(int width, int height) {
	return in_range(width, height) ? active_map->at(height).at(width) : TileType::BOOB;
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