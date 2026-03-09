#ifndef VIEWER_H
#define VIEWER_H

#include"tile.h"
#include"config.h"


class Viewer {
	MAPTYPE*& map_data;
public:
	Viewer(MAPTYPE*& data);
	~Viewer() = default;
	void set_data_source(MAPTYPE*& data);
	void print();
};

#endif
