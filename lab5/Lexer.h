#ifndef LEXER_H
#define LEXER_H
#include "Token.h"
#include <vector>
#include <string>
#include <sstream>
#include "Automaton.h"
#include "Automata.h"
#include <algorithm>

class Lexer {
private:
	std::vector<Automaton*> machines;
	std::vector<Token*> tokens;
	int lineNumber;
	int maxRead;
	int inputRead;
	int maxNewLines;

public:
	Lexer();
	Lexer(std::string input) : Lexer() {
		run(input);
		removeComments();
	}
	~Lexer() {
		for (auto& i : machines) {
			delete i;
		}
		for (auto& i : tokens) {
			delete i;
		}
	}

	void run(std::string& input);
	std::vector<Token*> get();
	void removeComments();
	std::string toString();
};

#endif
