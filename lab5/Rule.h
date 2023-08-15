#ifndef RULE_H
#define RULE_H
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "Predicate.h"
class Rule {
private:
	Predicate* head;
	std::vector<Predicate*> body;
public:
	~Rule() {
		delete head;
		for (auto& i : body) {
			delete i;
		}
	}

	std::vector<Predicate*> getBody() {
		return body;
	}

	Predicate* getHead() {
		return head;
	}

	std::string toString() const {
		std::stringstream s;
		s << *head << " :- ";
		for (auto& i : body) {
			s << *i << ",";
		}
		s.seekp(-1, std::ios_base::end);
		s << ".";
		return s.str();
	}
	operator std::string() const {
		return toString();
	}
	friend std::ostream& operator<<(std::ostream& os, const Rule& p) {
		return os << p.toString();
	}

	void addHead(Predicate* h) {
		head = h;
	}

	void addPredicate(Predicate* p) {
		body.push_back(p);
	}
};

#endif
