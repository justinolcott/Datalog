#ifndef TUPLE_H
#define TUPLE_H
#include <vector>
#include <string>
#include <sstream>

class Tuple {
private:
	std::vector<std::string> values;
public:
	Tuple(std::vector<std::string> body) : values(body) {}
	std::vector<std::string> copy() {
		return values;
	}
	void removeIndex(unsigned int i) {
		values.erase(values.begin() + i);
	}

	std::string toString() const {
		std::stringstream out;
		out << "(";
		for (std::string s : values) {
			out << s << ",";
		}
		out.seekp(-1, std::ios_base::end);
		out << ")";
		return out.str();
	}
	std::string front() const {
		return values.front();
	}
	int size() const {
		return values.size();
	}
	std::string at(unsigned int i) const {
		return values.at(i);
	}
	bool operator<(const Tuple& other) const {
		if (toString() < other.toString()) {
			return true;
		}
		return false;
	}
	bool operator==(Tuple& other) const {
		bool value = true;
		for (unsigned int i = 0; i < values.size(); i++) {
			if (values.at(i) != other.copy().at(i)) {
				value = false;
			}
		}
		return value;
	}

	void swap(unsigned int i, unsigned int j) {
		std::string temp = values.at(i);
		values.at(i) = values.at(j);
		values.at(j) = temp;
	}
};
#endif
