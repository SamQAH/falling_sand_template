#include"tile.h"

char to_display_char(TileType tp) {
	char display_char = '#';
	switch (tp)
	{
	case TileType::VOID:
		display_char = ' ';
		break;
	case TileType::GROUND:
		display_char = '=';
		break;
	case TileType::WATER:
		display_char = '.';
		break;
	case TileType::LAVA:
		display_char = 'L';
		break;
	case TileType::SAND:
		display_char = 's';
		break;
	case TileType::STICKY_POWDER:
		display_char = 'S';
		break;
	default:
		display_char = '#';
		break;
	}
	return display_char;
}

Location TileIter(std::function<TileType(int, int)> get) {
	TileType curr_tp = get(0, 0);
	return Location{ 0,0,curr_tp };
}