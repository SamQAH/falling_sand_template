#ifndef VIEWER_H
#define VIEWER_H

#include"tile.h"
#include"config.h"

void enableANSI();

class Viewer;

class Viewer {
	ostream& out;
	MAPTYPE*& map_data;
	list<string> log_once; // messages to output once, no newline characters in the messages
	int pre_num_logged; // probably can be deleted, to_start has the info
	string to_start; // moves cursor to the begining
public:
	Viewer(ostream& out, MAPTYPE*& data);
	~Viewer();
	void set_data_source(MAPTYPE*& data);
	void print();
	friend Viewer& operator<<(Viewer& viewer, const string& str);
};

Viewer& operator<<(Viewer& viewer, const string& str);

#endif
