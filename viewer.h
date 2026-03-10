#ifndef VIEWER_H
#define VIEWER_H

#include"tile.h"
#include"config.h"

void enableANSI();

class Viewer;

class Viewer {
	ostream& out;
	MAPTYPE*& map_data;
	list<string> log_once;
	int pre_num_logged;
public:
	Viewer(ostream& out, MAPTYPE*& data);
	~Viewer();
	void set_data_source(MAPTYPE*& data);
	void print();
	friend Viewer& operator<<(Viewer& viewer, const string& str);
};

Viewer& operator<<(Viewer& viewer, const string& str);

#endif
