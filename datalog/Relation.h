#ifndef RELATION_H
#define RELATION_H
#include <set>
#include <string>
#include <sstream>
#include <list>
#include <iterator>
#include "Tuple.h"
#include "Header.h"
#include "Parser.h"
class Relation {
private:
	struct APtrComp {
		bool operator()(const Tuple* lhs, const Tuple* rhs) const {
			return(*lhs < *rhs);
		}
	};
	std::set<Tuple*, APtrComp> tuples;
	std::string name;
	Header header;
public:
	Relation(std::string name, Header header) : name(name), header(header) {}
	Relation() {}
	Relation(std::string name, Header header, std::set<Tuple*, APtrComp> tuples) : tuples(tuples), name(name), header(header) {}
	~Relation() {
		for (auto& i : tuples) {
			delete i;
		}
	}
	unsigned int size() {
		return tuples.size();
	}

	void addTuple(Tuple* tuple) {
		tuples.insert(tuple);
	}

	void removeIndex(unsigned int i) {
		header.remove(i);
		for (auto& j : tuples) {
			j->removeIndex(i);
		}
	}
	void addName(std::string n) {
		name = n;
	}
	void addHeader(Header h) {
		header = h;
	}
	Header getHeader() {
		return header;
	}
	void addToHeader(std::string s) {
		header.add(s);
	}
	std::string getName() {
		return name;
	}
	std::string toString() const {
		std::stringstream out;
		for (Tuple* t : tuples) {
			if (header.size() > 0) { out << "  "; }
			for (unsigned int i = 0; i < header.size(); i++) {
				out << header.at(i) << "=" << t->at(i);
				if (i != header.size() - 1) {
					out << ", ";
				}
			}
			if (header.size() > 0) {
				out << std::endl;
			}
		}
		std::string o = out.str();
		return out.str();
	}

	Relation* selectValue(std::string index, std::string value) {
		Relation* r = new Relation();
		r->addHeader(Header(header.copy()));
		r->addName(name);
		unsigned int ind = 0;
		for (unsigned int i = 0; i < header.size(); i++) {
			if (header.at(i) == index) {
				ind = i;
			}
		}
		for (auto& i : tuples) {
			if (i->at(ind) == value) {
				r->addTuple(new Tuple(i->copy()));
			}
		}
		return r;
	}

	Relation* selectIndex(std::string index1, std::string index2) {
		Relation* r = new Relation();
		r->addHeader(header);
		r->addName(name);
		unsigned int ind1 = 0;
		unsigned int ind2 = 0;
		for (unsigned int i = 0; i < header.size(); i++) {
			if (header.at(i) == index1) {
				ind1 = i;
			}
			if (header.at(i) == index2) {
				ind2 = i;
			}
		}
		for (auto& i : tuples) {
			if (i->at(ind1) == i->at(ind2)) {
				r->addTuple(new Tuple(i->copy()));
			}
		}
		return r;
	}
	Relation* project(std::list<std::string> columns) {
		Relation* r = new Relation();
		r->addName(name);
		r->addHeader(Header(header.copy()));
		for (auto& i : tuples) {
			r->addTuple(new Tuple(i->copy()));
		}
		for (int i = header.size() - 1; i >= 0; i--) {
			bool keep = false;
			for (auto& j : columns) {
				if (header.at(i) == j) {
					keep = true;
				}
			}
			if (keep == false) {
				r->removeIndex(i);
			}
		}
		return r;
	}

	void swapHeader(unsigned int i, unsigned int j) {
		header.swap(i, j);
	}

	void swapTuples(unsigned int i, unsigned int j) {
		for (Tuple* k : tuples) {
			k->swap(i, j);
		}
	}
	Relation* projecttwo(std::list<std::string> columns) {
		Relation* r = project(columns);

		unsigned int ii = 0;
		for (std::string i : columns) {
			unsigned int jj = 0;
			for (std::string j : r->getHeader().get()) {
				if (j == i) {
					if (ii != jj) {
						r->swapTuples(ii, jj);
						r->swapHeader(ii, jj);
						break;
					}
				}
				jj++;
			}

			ii++;
		}
		return r;
	}

	Relation* rename(std::list<std::string> names) {
		Relation* r = new Relation();
		r->addName(name);
		r->addHeader(Header(names));
		for (auto& i : tuples) {
			r->addTuple(new Tuple(i->copy()));
		}
		return r;
	}
	Header combineHeaders(Header h1, Header h2) {
		std::list<std::string> names;
		for (std::string i : h1.get()) {
			names.push_back(i);
		}
		unsigned int jj = 0;
		for (std::string i : h2.get()) {
			bool dup = false;
			for (std::string j : h1.get()) {
				if (i == j) {
					dup = true;
				}
			}
			if (dup == false) {
				names.push_back(h2.at(jj));
			}
			jj++;
		}
		return Header(names);
	}

	bool isJoinable(Tuple* r1, Tuple* r2, Header h1, Header h2) {
		bool joinable = true;
		std::list<std::string> indexs;
		unsigned int ii = 0;
		for (std::string i : h1.get()) {
			unsigned int jj = 0;
			for (std::string j : h2.get()) {
				if (i == j) {
					if (r1->at(ii) != r2->at(jj)) {
						joinable = false;
					}
				}
				jj++;
			}
			ii++;
		}
		return joinable;
	}

	//Only call this one on the relation with combined header
	Tuple* combineTuples(Tuple* t1, Tuple* t2, Header h1, Header h2) {
		std::vector<std::string> tups;
		//Add all the tuples from the first
		for (unsigned int i = 0; i < h1.size(); i++) {
			tups.push_back(t1->at(i));
		}
		//Add only the tuples not in the first
		unsigned int jj = 0;
		for (std::string i : h2.get()) {
			bool dup = false;
			for (std::string j : h1.get()) {
				if (i == j) {
					dup = true;
				}
			}
			if (dup == false) {
				tups.push_back(t2->at(jj));
			}
			jj++;
		}
		return new Tuple(tups);
	}

	Relation* join(Relation* other) {
		//Combine headers
		Relation* rel = new Relation();
		rel->addHeader(combineHeaders(header, other->getHeader()));

		for (Tuple* i : tuples) {
			for (Tuple* j : other->getTuples()) {
				if (isJoinable(i, j, header, other->getHeader())) {
					rel->addTuple(combineTuples(i, j, header, other->getHeader()));
				}
			}
		}
		return rel;
	}

	Relation* uneon(Relation* other) {
		std::set<Tuple*, APtrComp> ot = other->getTuples();
		Relation* r = new Relation();
		r->addHeader(header);
		r->addName(name);
		for (std::set<Tuple*, APtrComp>::iterator it = ot.begin(); it != ot.end(); ++it) {
			if (tuples.insert(*it).second == true) {
				r->addTuple(*it);
			}
		}
		return r;
	}

	Relation* copy() {
		Relation* r = new Relation();
		r->addName(name);
		r->addHeader(Header(header));
		for (auto& i : tuples) {
			r->addTuple(new Tuple(i->copy()));
		}
		return r;
	}

	std::set<Tuple*, APtrComp> getTuples() {
		return tuples;
	}

	void addTuples(std::set<Tuple*, APtrComp> t) {
		tuples = t;
	}
};
#endif
