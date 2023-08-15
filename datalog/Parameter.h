#ifndef PARAMETER_H
#define PARAMETER_H
#include <string>
#include <iostream>

class Parameter {
public:
	virtual ~Parameter() {}
	virtual std::string toString() const = 0;
	virtual bool isConstant() = 0;
	virtual std::string get() = 0;
	operator std::string() const {
		return toString();
	}
	friend std::ostream& operator<<(std::ostream& os, const Parameter& p) {
		return os << p.toString();
	}
};

class Expression : public Parameter {
private:
	Parameter* left;
	Parameter* right;
	std::string op;
public:
	std::string toString() const {
		return "(" + (std::string)*left + op + (std::string)*right + ")";
	}
	std::string get() {
		return toString();
	}
	void addLeft(Parameter* l) {
		left = l;
	}
	void addRight(Parameter* r) {
		right = r;
	}
	void addOp(std::string o) {
		op = o;
	}
	bool isConstant() {
		return false;
	}
};

class Identifier : public Parameter {
private:
	std::string value;
public:
	Identifier(std::string v) : value(v) {}
	std::string toString() const {
		return value;
	}
	std::string get() {
		return value;
	}
	bool isConstant() {
		return false;
	}
};

class String : public Parameter {
private:
	std::string value;
public:
	String(std::string v) : value(v) {}
	std::string toString() const {
		return value;
	}
	std::string get() {
		return value;
	}
	bool isConstant() {
		return true;
	}
};

#endif

