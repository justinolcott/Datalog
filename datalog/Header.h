#ifndef HEADER_H
#define HEADER_H
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include "Parser.h"
class Header {
private:
	std::vector<std::string> names;
public:
	Header(std::vector<std::string> attributes) : names(attributes) {}
	Header(std::list<std::string> attributes) {
		for (auto& i : attributes) {
			names.push_back(i);
		}
	}

	Header() {}

	std::vector<std::string> get() {
		return names;
	}

	std::string at(unsigned int i) const {
		return names.at(i);
	}

	unsigned int size() const {
		return names.size();
	}

	std::vector<std::string> copy() {
		return names;
	}

	void remove(unsigned int i) {
		names.erase(names.begin() + i);
	}
	void add(std::string value) {
		names.push_back(value);
	}

	std::string toString() const {
		std::stringstream out;
		out << "(";
		for (auto& i : names) {
			out << i << ", ";
		}
		out.seekp(-1, std::ios_base::end);
		out << ")";
		return out.str();
	}

	unsigned int find(std::string input) {
		for (unsigned int i = 0; i < names.size(); i++) {
			if (input == names.at(i)) {
				return i;
			}
		}
		return -1;
	}

	void swap(unsigned int i, unsigned int j) {
		std::string temp = names.at(i);
		names.at(i) = names.at(j);
		names.at(j) = temp;
	}
};
#endif
