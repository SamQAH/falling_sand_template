#include"tile.h"
#include"logic.h"
#include"viewer.h"
#include"config.h"

int main() {
	Logic logic(10, 10);
	Viewer viewer(logic.get_active_map());

	logic.set_tile_at(3, 3, TileType::GROUND);
	logic.set_tiles_rect(4, 3, 2, 1, TileType::WATER);


	bool running = true;
	TileType tp = TileType::EMPTY;
	while (running) {
		//system("cls");
		viewer.print();
		string buffer;
		int temp_a;
		int temp_b;
		char temp_char = '\n';
		getline(cin, buffer);
		istringstream iss{buffer};
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
			}
			catch (int e) {
				cout << "Can not convert:" << buffer << " into TileType." << endl;
			}
			cout << "set tile " << to_string(tp) << " at " << temp_a << ", " << temp_b << endl;
			logic.set_tile_at(temp_a, temp_b, tp);
			break;
		default:
			auto time_start = chrono::high_resolution_clock::now();
			//Sleep(1); test timing works
			logic.step();
			auto time_end = chrono::high_resolution_clock::now();
			chrono::duration<double, std::milli> duration_ms = (time_end - time_start);
			cout << "step: " << duration_ms.count() << "ms" << endl;
			break;
		}
		
	}
	return 0;
}