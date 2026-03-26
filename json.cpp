#include "json.h"

bool getJsonString(istream& in, JsonString& str) {
	cerr << "get string: ";
	string buffer;
	getline(in, buffer, '\"');
	getline(in, buffer, '\"');
	if (in) {
		str.value = buffer;
		cerr << str.value << endl;
		return true;
	}
	return false;
}
bool getJsonDouble(istream& in, JsonDouble& db) {
	cerr << "get double: ";
	double temp;
	in >> temp;
	if (in) {
		db.value = temp;
		cerr << db.value << endl;
		return true;
	}
	return false;
}
bool getJsonTree(istream& in, JsonTree& tree) {
	cerr << "get tree: ";
	char next;
	in >> next;
	while (next != '}') {
		cerr << "get tree element ";
		char colon;
		JsonObject* key = nullptr;
		JsonObject* val = nullptr;
		in >> ws >> key >> ws >> colon >> ws >> val >> ws >> next;// TODO debug
		if (!in) {
			cerr << "in failed" << flush;
		}
		if (key == nullptr) {
			cerr << "key nullptr " << flush;
		}
		if (val == nullptr) {
			cerr << "val nullptr " << flush;

		}
		if (in && colon == ':' && (next == ',' || next == '}')) {
			cerr << "added element to tree";
			tree.value.emplace( static_cast<string>(*key), val);
		}
		else {
			return false;
		}
	}
	cerr << "get tree done" << endl;
	return true;
}
bool getJsonList(istream& in, JsonList& lst) {
	cerr << "get list: ";
	char next = in.get();
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
	cerr << "get list done" << endl;
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
		good = getJsonString(in, *((JsonString*)temp));
	}
	else {
		temp = new JsonDouble();
		good = getJsonDouble(in, (JsonDouble&)*temp);
	}

	if (!good || !in || !temp) {
		in.setstate(std::ios_base::badbit);
		cerr << "<< bad: " << flush;
		return in;
	}
	JsonObject* a = acc;
	acc = temp;
	temp = a;
	delete temp;
	string read_val = *acc;
	cerr << "<< read: " << read_val << flush;
	return in;
}

JsonTree::operator string() const
{
	ostringstream oss;
	oss << '{';
	for (auto iter = value.begin(); iter != value.end(); ++iter) {
		oss << iter->first << ':' << static_cast<string>(*(iter->second)) << ',' << endl;
	}
	oss << '}' << endl;
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
	oss << ']' << endl;
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
