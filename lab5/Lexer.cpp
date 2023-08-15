#include "Lexer.h"

Lexer::Lexer() {
	machines.push_back(new Comma);
	machines.push_back(new Period);
	machines.push_back(new QMark);
	machines.push_back(new LeftParen);
	machines.push_back(new RightParen);
	machines.push_back(new Colon);
	machines.push_back(new ColonDash);
	machines.push_back(new Multiply);
	machines.push_back(new Add);
	machines.push_back(new Schemes);
	machines.push_back(new Facts);
	machines.push_back(new Rules);
	machines.push_back(new Queries);
	machines.push_back(new MyString);
	machines.push_back(new ID);
	machines.push_back(new Comment);
	machines.push_back(new Undefined);

	this->lineNumber = 1;
	this->maxRead = 0;
	this->inputRead = 0;
	this->maxNewLines = 0;
}

std::vector<Token*> Lexer::get() {
	return tokens;
}

void Lexer::removeComments() {
	int size = tokens.size();
	for (int i = size - 1; i >= 0; i--) {
		if (tokens.at(i)->getType() == TokenType::COMMENT) {
			tokens.erase(tokens.begin() + i);
		}
	}
}

void Lexer::run(std::string& input) {
	lineNumber = 1;
	while (input.size() > 0) {
		maxRead = 0;
		Automaton* maxMachine = machines.front();
		Token* newToken = new Token();
		for (auto& machine : machines) {
			inputRead = machine->read(input);
			if (inputRead > maxRead) {
				maxRead = inputRead;
				maxMachine = machine;
				maxNewLines = machine->newLinesRead();
			}
		}
		if (maxRead > 0) {
			newToken = maxMachine->createToken(input.substr(0, maxRead), lineNumber);
			lineNumber += maxNewLines;
			tokens.push_back(newToken);
		}
		else {
			if (input.front() == '\n') {
				maxRead = 1;
				++lineNumber;
			}
			else if (input.front() == '\t' || input.front() == ' ') {
				maxRead = 1;
			}
			else {
				maxRead = 1;
				newToken = new Token(TokenType::UNDEFINED, input.substr(0, 1), lineNumber);
				tokens.push_back(newToken);
			}
		}
		input = input.substr(maxRead);
	}
	tokens.push_back(new Token(TokenType::EOFILE, "", lineNumber));
}

std::string Lexer::toString() {
	std::stringstream ss;
	for (auto& i : tokens) {
		ss << i->toString() << "\n";
	}
	ss << "Total Tokens = " << tokens.size();
	return ss.str();
}