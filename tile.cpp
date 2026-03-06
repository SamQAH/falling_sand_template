#include"tile.h"

char to_display_char(const TileType tp) {
	switch (tp)
	{
	case TileType::EMPTY:
		return ' ';
	case TileType::GROUND:
		return '=';
	case TileType::WATER:
		return '.';
	case TileType::LAVA:
		return 'L';
	case TileType::SAND:
		return 's';
	case TileType::STICKY_POWDER:
		return 'S';
	default:
		return '#';
	}
}

std::string to_string(const TileType tp) {
	switch (tp)
	{
	case TileType::BOOB:
		return "Bounds";
	case TileType::EMPTY:
		return "Void";
	case TileType::GROUND:
		return "Ground";
	case TileType::WATER:
		return "Water";
	case TileType::LAVA:
		return "Lava";
	case TileType::SAND:
		return "Sand";
	case TileType::STICKY_POWDER:
		return "Sticky Powder";
	default:
		return "Unknown";
	}
}

Location TileIter(std::function<TileType(int, int)> get) {
	TileType curr_tp = get(0, 0);
	if (get(0, 1) != TileType::BOOB) {
		return Location{ 0,1,curr_tp };
	}
	return Location{ 0,0,curr_tp };
}