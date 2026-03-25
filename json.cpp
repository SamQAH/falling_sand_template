#include "json.h"

bool getJsonString(istream& in, JsonString& str) {
	string buffer;
	getline(in, buffer, '\"');
	getline(in, buffer, '\"');
	if (in) {
		str.value = buffer;
		return true;
	}
	return false;
}
bool getJsonDouble(istream& in, JsonDouble& db) {
	double temp;
	in >> temp;
	if (in) {
		db.value = temp;
		return true;
	}
	return false;
}
bool getJsonTree(istream& in, JsonTree& tree) {
	char next = in.get();
	while (next != '}') {
		char colon = ' ';
		JsonObject* key = nullptr;
		JsonObject* val = nullptr;
		in >> key >> ws >> colon >> ws >> val >> ws >> next;
		if (in && colon == ':') {
			tree.value.emplace(key->to_string(), val);
		}
		else {
			return false;
		}
	}
	return true;
}
bool getJsonList(istream& in, JsonList& lst) {
	char next = in.get();
	while (in && next != ']') {
		JsonObject* temp = nullptr;
		in >> temp >> ws >> next;
		if (in) {
			lst.value.push_back(unique_ptr<JsonObject>(temp));
		}
		else {
			return false;
		}
	}
	return true;
}


istream& operator>>(istream& in, JsonObject* acc) {
	in >> ws;
	char next = in.peek();
	string buffer;
	bool good = false;
	JsonObject* temp = nullptr;
	if (next == '{') {
		temp = new JsonTree();
		good = getJsonTree(in, (JsonTree&)*temp);
	}
	else if (next == '[') {
		temp = new JsonList();
		good = getJsonList(in, (JsonList&)*temp);
	}
	else if (next == '\"') {
		temp = new JsonString();
		good = getJsonString(in, (JsonString&)*temp);
	}
	else {
		temp = new JsonDouble();
		good = getJsonDouble(in, (JsonDouble&)*temp);
	}
	if (!good || temp == nullptr) {
		return in;
	}
	if (!good || !in) {
		in.setstate(std::ios_base::badbit);
		return in;
	}
	swap(acc, temp);
	delete temp;
	return in;
}

string JsonTree::to_string() const
{
	ostringstream oss;
	oss << '{';
	for (auto iter = value.begin(); iter != value.end(); ++iter) {
		oss << iter->first << ':' << iter->second->to_string() << ',' << endl;
	}
	oss << '}' << endl;
	return oss.str();
}

JsonType JsonTree::get_data_type() const
{
	return JsonType::TREE;
}

string JsonList::to_string() const
{
	ostringstream oss;
	oss << '[';
	for (auto& iter : value) {
		oss << iter->to_string() << ',';
	}
	oss << ']' << endl;
	return oss.str();
}

JsonType JsonList::get_data_type() const
{
	return JsonType::LIST;
}

string JsonString::to_string() const
{
	return value;
}

JsonType JsonString::get_data_type() const
{
	return JsonType::STRING;
}

string JsonDouble::to_string() const
{
	return ::std::to_string(value);
}

JsonType JsonDouble::get_data_type() const
{
	return JsonType::DOUBLe;
}
