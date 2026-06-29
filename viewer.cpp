#include"viewer.h"
#include"tile.h"
#include"tile_manager.h"

void enableANSI() {
#ifdef _WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut != INVALID_HANDLE_VALUE) {
		DWORD dwMode = 0;
		if (GetConsoleMode(hOut, &dwMode)) {
			dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			if (!SetConsoleMode(hOut, dwMode)) {
				// Handle error if needed (e.g., older Windows version without support)
				cerr << "enable ANSI error!" << endl;
			}
		}
	}
#endif
}

Viewer::Viewer(ostream& out, MAPTYPE*& data) : out{ out }, map_data{ data }, log_once{ }, pre_num_logged{ 0 }, to_start{} {
	enableANSI();
	out << "Viewer created" << '\n';
}

Viewer::~Viewer() {
	// out << endl;
}

void Viewer::set_data_source(MAPTYPE*& data) {
	map_data = data;
}

void Viewer::print() {
	MAPTYPE& map = *map_data;
	int display_height = map.size();
	int display_width = map.at(0).size();
#ifndef DEBUG
	out << to_start;
#endif
	out << ' ';
	for (int i = 0; i < display_width; i++) {
		out << ' ' << i % 10;
	}
	out << '\n';
	int h = 0;
	for (auto& rows : map) {
		out << h++ % 10;
		for (auto& tile : rows) {
			out << ' ' << TileManager::at(tile)->get_displayChar();
		}
		out << '\n';
	}
	out << '\n';
	pre_num_logged = 0;
	for (auto& str : log_once) {
		out << str << "\n";
		pre_num_logged++;
	}
	out << flush;
	to_start = ("\x1B[" + to_string(display_height + pre_num_logged + 3) + "A\x1B[0J");
	log_once.clear();

}

Viewer& operator<<(Viewer& viewer, const string& str) {
	viewer.log_once.emplace_back(str);
	return viewer;
}