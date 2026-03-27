#include "json.h"

bool getJsonString(istream& in, JsonString& str) {
	str.value = "";
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
	db.value = 0;
	double temp;
	in >> temp;
	if (in) {
		db.value = temp;
		return true;
	}
	return false;
}
bool getJsonTree(istream& in, JsonTree& tree) {
	tree.value.clear();
	char next;
	in >> next >> ws;
	next = in.peek();
	while (next != '}') {
		char colon;
		JsonObject* key = nullptr;
		JsonObject* val = nullptr;
		in >> ws >> key >> ws >> colon >> ws >> val >> ws >> next;
		if (in && colon == ':' && (next == ',' || next == '}')) {
			tree.value.emplace( static_cast<string>(*key), val);
		}
		else {
			return false;
		}
	}
	return true;
}
bool getJsonList(istream& in, JsonList& lst) {
	lst.value.clear();
	char next;
	in >> next >> ws;
	next = in.peek();
	while (in && next != ']') {
		JsonObject* temp = nullptr;
		in >> ws >> temp >> ws >> next;
		if (in) {
			lst.value.push_back(unique_ptr<JsonObject>(temp));
		}
		else {
			return false;
		}
	}
	return true;
}


istream& operator>>(istream& in, JsonObject*& acc) {
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
		good = getJsonString(in, *((JsonString*)temp));
	}
	else if (next >= '0' && next <= '9') {
		temp = new JsonDouble();
		good = getJsonDouble(in, (JsonDouble&)*temp);
	}

	if (!good || !in || !temp) {
		in.setstate(std::ios_base::badbit);
		return in;
	}
	JsonObject* a = acc;
	acc = temp;
	temp = a;
	delete temp;
	string read_val = *acc;
	return in;
}

JsonTree::operator string() const
{
	ostringstream oss;
	oss << '{';
	for (auto iter = value.begin(); iter != value.end(); ++iter) {
		oss << iter->first << ':' << static_cast<string>(*(iter->second)) << ',';
	}
	oss << '}';
	return oss.str();
}

JsonType JsonTree::get_data_type() const
{
	return JsonType::TREE;
}

JsonList::operator string() const
{
	ostringstream oss;
	oss << '[';
	for (auto& iter : value) {
		oss << static_cast<string>(*iter) << ',';
	}
	oss << ']';
	return oss.str();
}

JsonType JsonList::get_data_type() const
{
	return JsonType::LIST;
}

JsonString::operator string() const
{
	return value;
}

JsonType JsonString::get_data_type() const
{
	return JsonType::STRING;
}

JsonDouble::operator string() const
{
	return ::std::to_string(value);
}

JsonType JsonDouble::get_data_type() const
{
	return JsonType::DOUBLe;
}

JsonNull::operator string() const
{
	return "nullptr";
}

JsonType JsonNull::get_data_type() const
{
	return JsonType::NUll;
}

JsonObject::operator string() const
{
	return "";
}

string JsonObject::to_string() const
{
	return static_cast<string>(*this);
}
