#include"config.h"

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
	MAPTYPE*& get_active_map();
	bool set_tile_at(int width, int height, TileType tp);
	TileType get_tile_at(int width, int height);
	bool set_tiles_rect(int width, int height, int wid, int hei, TileType tp);
	void set_get_tile_relative(int width, int height);
	TileType get_tile_at_relative(int width, int height);
	bool in_range(int width, int height);

private:
	int relative_width, relative_height;
};