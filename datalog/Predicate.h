#ifndef PREDICATE_H
#define PREDICATE_H
#include <string>
#include <vector>
#include <sstream>
#include "Parameter.h"

class Predicate {
private:
	std::string head;
	std::vector<Parameter*> body;
public:
	~Predicate() {
		for (auto& i : body) {
			delete i;
		}
	}
	std::string toString() const {
		std::stringstream s;
		s << head << "(";
		for (auto& i : body) {
			s << *i << ",";
		}
		s.seekp(-1, std::ios_base::end);
		s << ")";
		return s.str();
	}
	operator std::string() const {
		return toString();
	}
	friend std::ostream& operator<<(std::ostream& os, const Predicate& p) {
		return os << p.toString();
	}
	void addHead(std::string h) {
		head = h;
	}
	void addParameter(Parameter* parameter) {
		body.push_back(parameter);
	}
	int size() {
		return body.size();
	}

	std::string getHead() {
		return head;
	}

	Parameter* at(unsigned int i) {
		return body.at(i);
	}

	std::vector<std::string> getBody() {
		std::vector<std::string> out;
		for (auto& i : body) {
			out.push_back(i->toString());
		}
		return out;
	}

	std::vector<Parameter*> getParameters() {
		return body;
	}

	std::string domain() {
		std::stringstream s;
		for (auto& i : body) {
			s << "  " << i << std::endl;
		}
		return s.str();
	}
};
#endif
