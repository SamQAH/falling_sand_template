#ifndef LOGIC_H
#define LOGIC_H

#include"tile.h"
#include"config.h"

/* Logic coordinates a set of tiles 
 */
class Logic {
	int width, height;
	MAPTYPE map_1;
	MAPTYPE map_2;
	MAPTYPE* active_map;
	MAPTYPE* inactive_map;

public:
	Logic(int width, int height);
	~Logic() = default;
	void step();
	void full_step();
	MAPTYPE*& get_active_map();
	bool set_tile_at(int width, int height, TileType tp);
	TileType get_tile_at(int width, int height);
	bool set_tiles_rect(int width, int height, int wid, int hei, TileType tp);
	void set_get_tile_relative(int width, int height);
	TileType get_tile_at_relative(int width, int height);
	bool in_range(int width, int height);
	// TODO bool save_to_file(const string & filename);
	// TODO bool load_from_file(const string & filename);
private:
	int relative_width, relative_height;
	int step_count = 0;
	const int num_step_full = 16;
	int full_step_count = 0;
	bool isCurrentLogicStep(int row, int col);
};

#endif
