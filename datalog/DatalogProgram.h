#ifndef DATALOGPROGRAM_H
#define DATALOGPROGRAM_H
#include "MyException.h"
#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include <sstream>
#include "Predicate.h"
#include "Rule.h"

class DatalogProgram {
private:
	std::vector<Predicate*> schemes;
	std::vector<Predicate*> facts;
	std::vector<Rule*> rules;
	std::vector<Predicate*> queries;


public:
	DatalogProgram(std::vector<Predicate*> schemes,
		std::vector<Predicate*> facts,
		std::vector<Rule*> rules,
		std::vector<Predicate*> queries)
		: schemes(schemes), facts(facts), rules(rules), queries(queries) {}

	std::vector<Predicate*> getSchemes() {
		return schemes;
	}
	std::vector<Predicate*> getFacts() {
		return facts;
	}
	std::vector<Rule*> getRules() {
		return rules;
	}

	Rule* at(int i) {
		return rules.at(i);
	}

	std::vector<Predicate*> getQueries() {
		return queries;
	}

};
#endif

