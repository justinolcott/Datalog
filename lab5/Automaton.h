#ifndef AUTOMATON_H
#define AUTOMATON_H
#include <string>
#include <iostream>
#include "Token.h"

class Automaton {
protected:
	TokenType type;
	int newLines;
public:
	Automaton(TokenType type) : type(type), newLines(0) {}
	virtual ~Automaton() {}
	virtual int read(std::string& input) = 0;
	virtual Token* createToken(std::string input, int lineNumber) { return new Token(type, input, lineNumber); }
	virtual int newLinesRead() const { return newLines; }
};

#endif
