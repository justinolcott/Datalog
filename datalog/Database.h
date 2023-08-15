#ifndef DATABASE_H
#define DATABASE_H
#include <map>
#include <string>
#include <sstream>
#include "Relation.h"
class Database {
private:
	std::map<std::string, Relation*> relations;
public:
	Database() {}
	~Database() {
		for (auto& i : relations) {
			delete i.second;
		}
	}
	Relation* at(std::string i) {
		return relations[i];
	}
	void addRelation(Relation* relation) {
		relations.insert(std::pair<std::string, Relation*>(relation->getName(), relation));
	}
	std::string toString() {
		std::stringstream out;
		for (auto& i : relations) {
			out << i.second->toString() << std::endl;
		}
		return out.str();
	}
	void addTuple(std::string name, Tuple* tuple) {
		relations[name]->addTuple(tuple);
	}
};
#endif
