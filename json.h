#ifndef JSON_H
#define JSON_H
#define JSON_PARSE_H

#include "config.h"

enum JsonType {
	STRING,
	DOUBLe,
	TREE,
	LIST,
	JSON,
	NUll
};

class JsonObject {
protected:
	const string Indentation_String = "  ";
public:
	virtual operator string () const;
	virtual string to_string() const;
	virtual string to_string_helper(int num) const;
	virtual JsonType get_data_type() const;
	virtual ~JsonObject() = default;
};
class JsonNull : public JsonObject {
public:
	void* value;
	operator string () const;
	virtual JsonType get_data_type() const;
	virtual ~JsonNull() = default;
};
class JsonTree : public JsonObject {
protected:
	virtual string to_string_helper(int num) const;
public:
	map<string, unique_ptr<JsonObject>> value;
	operator string () const;
	virtual JsonType get_data_type() const;
	virtual ~JsonTree() = default;
};
class JsonList : public JsonObject {
protected:
	virtual string to_string_helper(int num) const;
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

// creates a JsonObject in acc from in, does nothing if parse is unsucessful
istream& operator>>(istream& in, JsonObject*& acc);

unique_ptr<JsonObject> file_to_json(const string& filename);

#endif