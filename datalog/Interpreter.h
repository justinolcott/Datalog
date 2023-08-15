#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include "DatalogProgram.h"
#include "Database.h"
#include "Graph.h"

class Interpreter {
private:
	DatalogProgram data;
	Database dataBase;
	std::vector<Relation*> queries;
public:
	Interpreter(DatalogProgram& data) : data(data) {}
	~Interpreter() {
		for (auto& i : queries) {
			delete i;
		}
	}
	std::string toString() {
		std::stringstream out;
		for (unsigned int i = 0; i < queries.size(); i++) {
			out << data.getQueries().at(i)->toString() << "? ";
			if (queries.at(i)->getTuples().size() == 0) {
				out << "No" << std::endl;
			}
			else {
				out << "Yes(" << queries.at(i)->getTuples().size() << ")" << std::endl;
				out << queries.at(i)->toString();
			}
		}
		return out.str();
	}
	void schemes() {
		for (Predicate* i : data.getSchemes()) {
			dataBase.addRelation(new Relation(i->getHead(), Header(i->getBody())));
		}
	}

	void facts() {
		for (Predicate* i : data.getFacts()) {
			dataBase.addTuple(i->getHead(), new Tuple(i->getBody()));
		}
	}

	void rules() {
		std::cout << "Rule Evaluation" << std::endl;
		//For each Rule
		bool changed = true;
		unsigned int passes = 0;
		while (changed == true) {
			changed = false;
			for (Rule* i : data.getRules()) {
				std::stringstream out;
				out << i->getHead()->toString() << " :- ";
				for (Predicate* j : i->getBody()) {
					out << j->toString() << ",";
				}
				out.seekp(-1, std::ios_base::end);
				out << "." << std::endl;
				std::cout << out.str();


				//Step 1
				std::vector<Relation*> relations;
				for (Predicate* j : i->getBody()) {
					relations.push_back(evaluatePredicate(j));
				}

				//Step 2
				Relation* inter = relations.at(0);
				for (unsigned int j = 0; j < relations.size() - 1; j++) {
					inter = inter->join(relations.at(j + 1));
				}

				//Step 3
				std::list<std::string> names;
				for (std::string j : i->getHead()->getBody()) {
					names.push_back(j);
				}
				inter = inter->projecttwo(names);

				//Step 4
				std::vector<std::string> renames;
				for (Predicate* j : data.getSchemes()) {
					if (j->getHead() == i->getHead()->getHead()) {
						renames = j->getBody();
						//std::cout << "Orig" << orig << std::endl;
						break;
					}
				}

				std::list<std::string> rerenames;
				for (std::string j : renames) {
					rerenames.push_back(j);
				}

				inter = inter->rename(rerenames);

				unsigned int oldSize = dataBase.at(i->getHead()->getHead())->size();
				//Step 5
				inter = dataBase.at(i->getHead()->getHead())->uneon(inter);
				std::cout << inter->toString();


				//check size
				unsigned int newSize = dataBase.at(i->getHead()->getHead())->size();
				if (oldSize != newSize) {
					changed = true;
				}
				//Delete unused relations
			}
			passes++;
		}
		std::cout << std::endl;
		std::cout << "Schemes populated after " << passes << " passes through the Rules." << std::endl;
		std::cout << std::endl;
	}

	void ruleOpt() {
		Graph g = Graph(data.getRules());
		std::cout << "Dependency Graph" << std::endl;
		std::cout << g.toString() << std::endl;
		g.dfsForest();
		g.findSCC();
		std::vector<std::pair<int, std::set<int> > > sccs = g.getSCCs();
		std::cout << "Rule Evaluation" << std::endl;
		//std::cout << sccs.size();
		for (auto i : sccs) {
			//one not dependent on itself
			std::cout << "SCC: ";
			std::string delim = "";
			for (auto j : i.second) {
				std::cout << delim << "R" << j;
				delim = ",";
			}
			std::cout << std::endl;

			bool found = false;

			for (auto j : data.at(i.first)->getBody()) {
				if (j->getHead() == data.at(i.first)->getHead()->getHead()) {
					found = true;
				}
			}

			if (!found && i.second.size() == 1) {
				evaluateRule(i.first);
			}
			else {
				evaluateRules(i);
			}
		}
	}

	void evaluateRules(std::pair<int, std::set<int> > scc) {
		//For each Rule
		bool changed = true;
		unsigned int passes = 0;
		while (changed == true) {
			changed = false;
			for (auto j : scc.second) {
				Rule* i = data.at(j);
				std::stringstream out;
				out << i->getHead()->toString() << " :- ";
				for (Predicate* j : i->getBody()) {
					out << j->toString() << ",";
				}
				out.seekp(-1, std::ios_base::end);
				out << "." << std::endl;
				std::cout << out.str();


				//Step 1
				std::vector<Relation*> relations;
				for (Predicate* j : i->getBody()) {
					relations.push_back(evaluatePredicate(j));
				}

				//Step 2
				Relation* inter = relations.at(0);
				for (unsigned int j = 0; j < relations.size() - 1; j++) {
					inter = inter->join(relations.at(j + 1));
				}

				//Step 3
				std::list<std::string> names;
				for (std::string j : i->getHead()->getBody()) {
					names.push_back(j);
				}
				inter = inter->projecttwo(names);

				//Step 4
				std::vector<std::string> renames;
				for (Predicate* j : data.getSchemes()) {
					if (j->getHead() == i->getHead()->getHead()) {
						renames = j->getBody();
						//std::cout << "Orig" << orig << std::endl;
						break;
					}
				}

				std::list<std::string> rerenames;
				for (std::string j : renames) {
					rerenames.push_back(j);
				}

				inter = inter->rename(rerenames);

				unsigned int oldSize = dataBase.at(i->getHead()->getHead())->size();
				//Step 5
				inter = dataBase.at(i->getHead()->getHead())->uneon(inter);
				std::cout << inter->toString();


				//check size
				unsigned int newSize = dataBase.at(i->getHead()->getHead())->size();
				if (oldSize != newSize) {
					changed = true;
				}
				//Delete unused relations
			}
			passes++;
		}
		std::cout << passes << " passes: ";
		std::string delim = "";
		for (auto j : scc.second) {
			std::cout << delim << "R" << j;
			delim = ",";
		}
		std::cout << std::endl;
	}

	void evaluateRule(int node) {
		//For each Rule
		Rule* i = data.at(node);
		std::stringstream out;
		out << i->getHead()->toString() << " :- ";
		for (Predicate* j : i->getBody()) {
			out << j->toString() << ",";
		}
		out.seekp(-1, std::ios_base::end);
		out << "." << std::endl;
		std::cout << out.str();


		//Step 1
		std::vector<Relation*> relations;
		for (Predicate* j : i->getBody()) {
			relations.push_back(evaluatePredicate(j));
		}

		//Step 2
		Relation* inter = relations.at(0);
		for (unsigned int j = 0; j < relations.size() - 1; j++) {
			inter = inter->join(relations.at(j + 1));
		}

		//Step 3
		std::list<std::string> names;
		for (std::string j : i->getHead()->getBody()) {
			names.push_back(j);
		}
		inter = inter->projecttwo(names);

		//Step 4
		std::vector<std::string> renames;
		for (Predicate* j : data.getSchemes()) {
			if (j->getHead() == i->getHead()->getHead()) {
				renames = j->getBody();
				//std::cout << "Orig" << orig << std::endl;
				break;
			}
		}

		std::list<std::string> rerenames;
		for (std::string j : renames) {
			rerenames.push_back(j);
		}

		inter = inter->rename(rerenames);

		//unsigned int oldSize = dataBase.at(i->getHead()->getHead())->size();
		//Step 5
		inter = dataBase.at(i->getHead()->getHead())->uneon(inter);
		std::cout << inter->toString();


		//check size
		//Delete unused relations
		std::cout << "1 passes: R" << node << std::endl;
		//std::cout << "Schemes populated after " << passes << " passes through the Rules." << std::endl;
	}

	void test() {
		Graph g = Graph(data.getRules());
		std::cout << g.toString() << std::endl;
		g.dfsForest();
		std::cout << g.PO() << std::endl;
		g.findSCC();

	}

	void query() {
		for (auto& i : data.getQueries()) {
			queries.push_back(evaluatePredicate(i));
		}

		std::stringstream out;
		for (unsigned int i = 0; i < queries.size(); i++) {
			out << data.getQueries().at(i)->toString() << "? ";
			if (queries.at(i)->getTuples().size() == 0) {
				out << "No" << std::endl;
			}
			else {
				out << "Yes(" << queries.at(i)->getTuples().size() << ")" << std::endl;
				out << queries.at(i)->toString();
			}
		}
		std::cout << "Query Evaluation" << std::endl;
		std::cout << out.str() << std::endl;
	}

	Relation* evaluatePredicate(Predicate* p) {
		//get relation
		Relation* s = dataBase.at(p->getHead());
		Relation* r = new Relation(s->getName(), s->getHeader(), s->getTuples());
		unsigned int k = 0;
		std::map<std::string, unsigned int> variables;
		std::list<std::string> columns;
		std::list<std::string> names;
		//for all parameters
		for (auto& i : (p->getParameters())) {
		//if constant
			if (i->isConstant()) {
				r = r->selectValue(r->getHeader().at(k), i->get()); //new relation is created that needs to be deleted
			}
		//if variable
			else {
				//if its new
				if (variables.find(i->get()) == variables.end()) {
					variables.insert({ i->get(), k });
					names.push_back(i->get());
					columns.push_back(r->getHeader().at(k));
				}
				//if its old
				else {
					r = r->selectIndex(r->getHeader().at(variables[i->get()]), r->getHeader().at(k));
				}
			}
			k++;
		}
		r = r->project(columns);
		r = r->rename(names); //new relation is created that needs to be deleted
		return r;
	}
};

#endif
