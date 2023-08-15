#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include <sstream>
#include <set>
#include <algorithm>
#include "DatalogProgram.h"
#include "Token.h"
#include "MyException.h"

class Parser {
private:
	std::vector<Token*> tokens;
	std::vector<Predicate*> schemes;
	std::vector<Predicate*> facts;
	std::vector<Rule*> rules;
	std::vector<Predicate*> queries;
	std::set<std::string> domain;
	unsigned int iter;
	std::string status;
	bool success;

	std::string tokenString(TokenType type);
	void match(TokenType type) {
		if (type != tokens.at(iter)->getType()) {
			throw MyException(tokens.at(iter)->toString());
		}
		iter++;
	}

	std::string matchString(TokenType type) {
		if (type != tokens.at(iter)->getType()) {
			throw MyException(tokens.at(iter)->toString());
		}
		return tokens.at(iter++)->get();
	}

	TokenType next() {
		return tokens.at(iter)->getType();
	}

public:
	Parser(std::vector<Token*> tokens) : tokens(tokens), iter(0), status(""), success(false) {
		dataLogProgram();
	}
	~Parser() {
		for (auto& i : schemes) {
			delete i;
		}
		for (auto& i : facts) {
			delete i;
		}
		for (auto& i : rules) {
			delete i;
		}
		for (auto& i : queries) {
			delete i;
		}
	}

	DatalogProgram run() {
		return DatalogProgram(schemes, facts, rules, queries);
	}

	std::vector<Predicate*> getSchemes() {
		return schemes;
	}

	std::vector<Predicate*> getFacts() {
		return facts;
	}

	std::vector<Predicate*> getQueries() {
		return queries;
	}

	std::vector<Rule*> getRules() {
		return rules;
	}

	std::set<std::string> getDomain() {
		return domain;
	}

	std::string toString() {
		std::stringstream ss;
		ss << status << std::endl;
		if (success) {
			ss << "Schemes(" << schemes.size() << "):" << std::endl;
			for (auto& i : schemes) {
				ss << "  " << *i << std::endl;
			}
			ss << "Facts(" << facts.size() << "):" << std::endl;
			for (auto& i : facts) {
				ss << "  " << *i << "." << std::endl;
			}
			ss << "Rules(" << rules.size() << "):" << std::endl;
			for (auto& i : rules) {
				ss << "  " << *i << std::endl;
			}
			ss << "Queries(" << queries.size() << "):" << std::endl;
			for (auto& i : queries) {
				ss << "  " << *i << "?" << std::endl;
			}
			ss << "Domain(" << domain.size() << "):" << std::endl;
			for (std::set<std::string>::iterator it = domain.begin(); it != domain.end(); it++) {
				ss << "  \'" << *it << "\'" << std::endl;
			}
		}
		return ss.str();
	}
	void dataLogProgram();
	void schemeList();
	void factList();
	void ruleList();
	void queryList();
	void scheme();
	void fact();
	void rule();
	void query();
	Predicate* headPredicate();
	Predicate* predicate();
	void predicateList(Rule* r);
	void parameterList(Predicate* p);
	void stringList(Predicate* p);
	void idList(Predicate* p);
	Parameter* parameter();
	Expression* expression();
	std::string myOperator();
};
#endif

