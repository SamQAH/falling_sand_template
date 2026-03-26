#ifndef JSON_H
#define JSON_H

#include "config.h"

enum JsonType {
	STRING,
	DOUBLe,
	TREE,
	LIST,
	JSON
};

class JsonObject {
public:
	virtual operator string () const = 0;
	virtual JsonType get_data_type() const = 0;
	virtual ~JsonObject() = default;
};
class JsonTree : public JsonObject {
public:
	map<string, unique_ptr<JsonObject>> value;
	operator string () const;
	virtual JsonType get_data_type() const;
	virtual ~JsonTree() = default;
};
class JsonList : public JsonObject {
public:
	list<unique_ptr<JsonObject>> value;
	operator string () const;
	virtual JsonType get_data_type() const;
	virtual ~JsonList() = default;
};
class JsonString : public JsonObject {
public:
	string value;
	operator string () const;
	virtual JsonType get_data_type() const;
	virtual ~JsonString() = default;
};
class JsonDouble : public JsonObject {
public:
	double value;
	operator string () const;
	virtual JsonType get_data_type() const;
	virtual ~JsonDouble() = default;
};

istream& operator>>(istream& in, JsonObject* acc);

#endif