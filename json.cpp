#include "json.h"

bool getJsonString(istream& in, JsonString& str) {
	if (!in) {
		cerr << "Received failed istream at getJsonString." << endl;
	}
	str.value = "";
	string buffer;
	getline(in, buffer, '\"');
	getline(in, buffer, '\"');
	if (in) {
		str.value = buffer;
		return true;
	}
	cerr << "Unexpected String read fail." << endl;
	return false;
}
bool getJsonDouble(istream& in, JsonDouble& db) {
	if (!in) {
		cerr << "Received failed istream at getJsonDouble." << endl;
	}
	db.value = 0;
	double temp;
	in >> temp;
	if (in) {
		db.value = temp;
		return true;
	}
	cerr << "Unexpected Double read fail." << endl;
	return false;
}
bool getJsonTree(istream& in, JsonTree& tree) {
	if (!in) {
		cerr << "Received failed istream at getJsonTree." << endl;
	}
	ostringstream error_str;
	bool failed = false;
	tree.value.clear();
	char next;
	in >> next >> ws;
	next = in.peek();
	if (next == '}') {
		in >> next;
	}
	while (in && next != '}') {
		char colon;
		JsonObject* key = nullptr;
		JsonObject* val = nullptr;
		in >> key >> ws;
		if (!in && !failed) {
			error_str << "key read failed.";
			failed = true;
		}
		in >> colon >> ws;
		if (!in && !failed) {
			error_str << "colon read failed.";
			failed = true;
		}
		in >> val >> ws;
		if (!in && !failed) {
			error_str << "value read failed.";
			failed = true;
		}
		in >> next >> ws;
		if (!in && !failed) {
			error_str << "separator character read failed.";
			failed = true;
		}
		if (in && colon == ':' && (next == ',' || next == '}')) {
			tree.value.emplace( static_cast<string>(*key), val);
		}
		else {
			cerr << "Unexpected Tree read fail.";
			if (colon != ':') {
				cerr << "Expected : , \'" << colon << "\' found.";
			}
			else if (next != ',' && next != '}') {
				cerr << "Expected , or } , \'" << colon << "\' found.";
			}
			else {
				cerr << "failed read.";
			}
			cerr << error_str.str() << endl;
			return false;
		}
	}
	return true;
}
bool getJsonList(istream& in, JsonList& lst) {
	if (!in) {
		cerr << "Received failed istream at getJsonList." << endl;
	}
	lst.value.clear();
	char next;
	in >> next >> ws;
	next = in.peek();
	if (next == ']') {
		in >> next;
	}
	while (in && next != ']') {
		JsonObject* temp = nullptr;
		in >> temp >> ws >> next >> ws;
		if (in && (next == ',' || next == ']')) {
			lst.value.push_back(unique_ptr<JsonObject>(temp));
		}
		else {
			cerr << "Unexpected List read fail." << endl;
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
	else if ((next >= '0' && next <= '9') || next == '-') {
		temp = new JsonDouble();
		good = getJsonDouble(in, (JsonDouble&)*temp);
	}
	else {
		cerr << "Unexpected character " << next << endl;
		good = false;
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

string JsonTree::to_string_helper(int num) const
{
	string indent = "";
	for (int i = 0; i < num; i++) {
		indent += JsonObject::Indentation_String;
	}
	ostringstream oss;
	oss << "{\n";
	for (auto iter = value.begin(); iter != value.end(); ++iter) {
		oss << indent << JsonObject::Indentation_String << iter->first << ':' << iter->second->to_string_helper(num + 1) << ",\n";
	}
	oss << indent << "}";
	return oss.str();
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

string JsonList::to_string_helper(int num) const
{
	string indent = "";
	for (int i = 0; i < num; i++) {
		indent += JsonObject::Indentation_String;
	}
	ostringstream oss;
	oss << "[\n";
	for (auto& iter : value) {
		oss << indent << JsonObject::Indentation_String << iter->to_string_helper(num + 1) << ",\n";
	}
	oss << indent << "]";
	return oss.str();
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
	return this->to_string_helper(0);
}

string JsonObject::to_string_helper(int num) const
{
	return static_cast<string>(*this);
}

JsonType JsonObject::get_data_type() const
{
	return JsonType::JSON;
}
