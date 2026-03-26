#include"tile.h"
#include"logic.h"
#include"viewer.h"
#include"json.h"
#include"config.h"
void test() {
	JsonObject* obj = nullptr;
	ifstream ifs{ "tile_properties.json" };
	if (!(ifs.good())) {
		cerr << "can not open json" << endl;
	}
	else {
		cerr << "opened json" << endl;
		ifs >> obj;
		cerr << static_cast<string>(*obj);
		ifs.close();
	}

}
int main() {
	test();
	//cout << "Start." << endl;
	Logic logic(20, 15);
	Viewer viewer(cout, logic.get_active_map());

	logic.set_tiles_rect(2, 1, 16, 3, TileType::WATER);
	logic.set_tiles_rect(5, 5, 10, 7, TileType::GROUND);
	logic.set_tiles_rect(6, 6, 8, 4, TileType::LAVA);

	bool running = true;
	TileType tp = TileType::EMPTY;
	while (running) {
		viewer.print();
		string buffer;
		int temp_a;
		int temp_b;
		char temp_char = '\n';
		auto time_start = chrono::high_resolution_clock::now();
		auto time_end = chrono::high_resolution_clock::now();
		chrono::duration<double, std::milli> duration_ms = (time_end - time_start);
		bool use_full_step = true;

		getline(cin, buffer);
		istringstream iss{ buffer };
		iss >> temp_char;
		switch (temp_char)
		{
		case 'd':
			running = false;
			break;
		case 's':
			iss >> temp_a >> temp_b >> buffer;
			try {
				tp = string_tile.at(buffer);
				viewer << ("set tile " + to_string(tp) + " at " + to_string(temp_a) + ", " + to_string(temp_b));
				logic.set_tile_at(temp_a, temp_b, tp);
			}
			catch (const out_of_range& e) {
				viewer << ("Can not convert:" + buffer + " into TileType.");
			}
			break;
		case 't':
			iss >> temp_a;
			time_start = chrono::high_resolution_clock::now();
			for (int i = 0; i < temp_a; i++) {
				logic.full_step();
			}
			time_end = chrono::high_resolution_clock::now();
			duration_ms = (time_end - time_start);
			viewer << ("sprint: " + to_string(duration_ms.count()) + "ms");
			break;
		case 'k':
			use_full_step = !use_full_step;
			break;
		default:
			time_start = chrono::high_resolution_clock::now();
			use_full_step ? logic.full_step() : logic.step();
			time_end = chrono::high_resolution_clock::now();
			duration_ms = (time_end - time_start);
			viewer << ("step: " + to_string(duration_ms.count()) + "ms");
			break;
		}
	}
}
