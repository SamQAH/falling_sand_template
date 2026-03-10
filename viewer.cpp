#include"viewer.h"
#include"tile.h"

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

Viewer::Viewer(ostream& out, MAPTYPE*& data) : out { out }, map_data { data }, log_once { }, pre_num_logged { 0 } {
	enableANSI();
	for (size_t i = 0; i < map_data->size() + 4; i++) {
		out << '\n';
	}
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
	int h = 0;
	out << "\x1B[" << display_height + pre_num_logged + 3 << "A\x1B[0J";
	out << ' ';
	for (int i = 0; i < display_width; i++) {
		out << ' ' << i % 10;
	}
	out << '\n';
	for (auto& rows : map) {
		out << h++ % 10;
		for (auto& tile : rows) {
			out << ' ' << to_display_char(tile);
		}
		out << '\n';
	}
	out << '\n';
	pre_num_logged = 0;
	for (auto& str : log_once) {
		out << str << endl;
		pre_num_logged++;
	}
	log_once.clear();

}

Viewer& operator<<(Viewer& viewer, const string& str) {
	viewer.log_once.emplace_back(str);
	return viewer;
}