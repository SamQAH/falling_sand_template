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
		string temp_word = "\n";
		auto time_start = chrono::high_resolution_clock::now();
		auto time_end = chrono::high_resolution_clock::now();
		chrono::duration<double, std::milli> duration_ms = (time_end - time_start);
		bool use_full_step = true;
		cout << ">>";
		getline(cin, buffer);
		istringstream iss{ buffer };
		iss >> temp_word;
		if (temp_word == "help") {
			viewer << "printing help message";
			viewer << "[enter] to tick once";
			viewer << "quit \tto stop the program";
			viewer << "set <row> <col> <name> \tsets the tile <name> at the coordinates";
			viewer << "sprint <n> \t ticks n times";
			viewer << "k \tswitches tick to not tick all locations once";
			viewer << "save <filename> \tto save the current scene to a file";
			viewer << "load <filename> \tto load a scene from filename";
		} 
		else if (temp_word == "quit" || temp_word == "exit" || temp_word == "stop" || temp_word == "d" || temp_word == "q") {
			running = false;
		} 
		else if (temp_word == "set") {
			iss >> temp_a >> temp_b >> buffer;
			try {
				tp = TileManager::find(buffer);
				viewer << ("set tile " + to_string(tp) + " at " + to_string(temp_a) + ", " + to_string(temp_b));
				logic.set_tile_at(temp_a, temp_b, tp);
			}
			catch (const out_of_range& e) {
				viewer << ("Can not convert:" + buffer + " into TileType.");
			}
		} 
		else if (temp_word == "sprint") {
			iss >> temp_a;
			time_start = chrono::high_resolution_clock::now();
			for (int i = 0; i < temp_a; i++) {
				logic.full_step();
			}
			time_end = chrono::high_resolution_clock::now();
			duration_ms = (time_end - time_start);
			viewer << ("sprint: " + to_string(duration_ms.count()) + "ms");
		} 
		else if (temp_word == "k") {
			use_full_step = !use_full_step;
			break;
		} 
		else if (temp_word == "save") {
			iss >> buffer;
			try {
				logic.save_to_file(buffer);
			}
			catch (const exception& e) {
				viewer << e.what();
			}
		} 
		else if (temp_word == "load") {
			iss >> buffer;
			try {
				logic.load_from_file(buffer);
			}
			catch (const exception& e) {
				viewer << e.what();
			}
			viewer.set_data_source(logic.get_active_map());
		}
		else if (temp_word == "new") {
			iss >> temp_a >> temp_b;
			logic = Logic(temp_a, temp_b);
			viewer.set_data_source(logic.get_active_map());
		}
		else if (temp_word == "\n") {
			time_start = chrono::high_resolution_clock::now();
			use_full_step ? logic.full_step() : logic.step();
			time_end = chrono::high_resolution_clock::now();
			duration_ms = (time_end - time_start);
			viewer << ("step: " + to_string(duration_ms.count()) + "ms");
		} 
		else {
			viewer << ("unknown command: " + temp_word);
		}
	}
}
