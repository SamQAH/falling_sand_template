#include"tile.h"
#include"tile_manager.h"
#include"logic.h"
#include"viewer.h"
#include"json.h"
#include"config.h"

void load_tiles() {
	TileManager::add("tile_properties.json");
	TileManager::add_reactions("tile_reactions.json");
#ifdef DEBUG
	cout << TileManager::to_string() << endl;
#endif
}
int main() {
	load_tiles();
	//cout << "Start." << endl;
	Logic logic(20, 15);
	Viewer viewer(cout, logic.get_active_map());

	logic.set_tiles_rect(2, 1, 16, 3, tile(water));
	logic.set_tiles_rect(5, 5, 10, 7, tile(ground));
	logic.set_tiles_rect(6, 6, 8, 4, tile(lava));

	bool running = true;
	TileType tp = tile(empty);
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
		cout << ">>";
		getline(cin, buffer);
		istringstream iss{ buffer };
		iss >> temp_char;
		switch (temp_char)
		{
			/* TODO save and load map from file
			*/
		case 'h':
			cout << "printing help message" << endl;
			cout << "[enter] to tick once" << endl;
			cout << "s <row> <col> <name> \tsets the tile <name> at the coordinates" << endl;
			cout << "t <n> \t ticks n times" << endl;
			cout << "k \tswitches tick to tick all locations once" << endl;
			break;
		case 'd':
			running = false;
			break;
		case 's':
			iss >> temp_a >> temp_b >> buffer;
			try {
				tp = TileManager::find(buffer);
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
