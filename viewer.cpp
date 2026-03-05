#include"viewer.h"

Viewer::Viewer(MAPTYPE*& data) : map_data{data} {}

void Viewer::set_data_source(MAPTYPE*& data) {
	map_data = data;
}

void Viewer::print() {
	MAPTYPE& map = *map_data;
	int display_height = map.size();
	int display_width = map.at(0).size();
	int h = 0;
	cout << ' ';
	for (int i = 0; i < display_width; i++) {
		cout << ' ' << i % 10;
	}
	cout << '\n';
	for (auto& rows : map) {
		cout << h++ % 10;
		for (auto& tile : rows) {
			cout << ' ' << to_display_char(tile);
		}
		cout << '\n';
	}
	cout << endl;
}