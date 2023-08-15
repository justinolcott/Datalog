#ifndef AUTOMATA_H
#define AUTOMATA_H

#include "Automaton.h"
#include "Token.h"
#include <string>

class Comma : public Automaton {
public:
	Comma() : Automaton(TokenType::COMMA) {}
	int read(std::string& input) {
		if (!input.empty() && input.at(0) == ',') {
			return 1;
		}
		return 0;
	}
};

class Period : public Automaton {
public:
	Period() : Automaton(TokenType::PERIOD) {}
	int read(std::string& input) {
		if (!input.empty() && input.at(0) == '.') {
			return 1;
		}
		return 0;
	}
};


class QMark : public Automaton {
public:
	QMark() : Automaton(TokenType::Q_MARK) {}
	int read(std::string& input) {
		if (!input.empty() && input.at(0) == '?') {
			return 1;
		}
		return 0;
	}
};

class LeftParen : public Automaton {
public:
	LeftParen() : Automaton(TokenType::LEFT_PAREN) {}
	int read(std::string& input) {
		if (!input.empty() && input.at(0) == '(') {
			return 1;
		}
		return 0;
	}
};

class RightParen : public Automaton {
public:
	RightParen() : Automaton(TokenType::RIGHT_PAREN) {}
	int read(std::string& input) {
		if (!input.empty() && input.at(0) == ')') {
			return 1;
		}
		return 0;
	}
};

class Colon : public Automaton {
public:
	Colon() : Automaton(TokenType::COLON) {}
	int read(std::string& input) {
		if (!input.empty() && input.at(0) == ':') {
			return 1;
		}
		return 0;
	}
};

class ColonDash : public Automaton {
public:
	ColonDash() : Automaton(TokenType::COLON_DASH) {}
	int read(std::string& input) {
		if (!input.empty() && input.substr(0, 2).compare(":-") == 0) {
			return 2;
		}
		return 0;
	}
};

class Multiply : public Automaton {
public:
	Multiply() : Automaton(TokenType::MULTIPLY) {}
	int read(std::string& input) {
		if (!input.empty() && input.at(0) == '*') {
			return 1;
		}
		return 0;
	}
};

class Add : public Automaton {
public:
	Add() : Automaton(TokenType::ADD) {}
	int read(std::string& input) {
		if (!input.empty() && input.at(0) == '+') {
			return 1;
		}
		return 0;
	}
};

class Schemes : public Automaton {
public:
	Schemes() : Automaton(TokenType::SCHEMES) {}
	int read(std::string& input) {
		if (!input.empty() && input.substr(0, 7).compare("Schemes") == 0) {
			return 7;
		}
		return 0;
	}
};


class Facts : public Automaton {
public:
	Facts() : Automaton(TokenType::FACTS) {}
	int read(std::string& input) {
		if (!input.empty() && input.substr(0, 5).compare("Facts") == 0) {
			return 5;
		}
		return 0;
	}
};

class Rules : public Automaton {
public:
	Rules() : Automaton(TokenType::RULES) {}
	int read(std::string& input) {
		if (!input.empty() && input.substr(0, 5).compare("Rules") == 0) {
			return 5;
		}
		return 0;
	}
};

class Queries : public Automaton {
public:
	Queries() : Automaton(TokenType::QUERIES) {}
	int read(std::string& input) {
		if (!input.empty() && input.substr(0, 7).compare("Queries") == 0) {
			return 7;
		}
		return 0;
	}
};

//Class for string
class MyString : public Automaton {
public:
	MyString() : Automaton(TokenType::STRING) {}
	int read(std::string& input) {
		newLines = 0;
		//If string is not empty and starts with a '
		if (!input.empty() && input.at(0) == '\'') {
			int i = 1;
			//while string is not empty
			while (input[i] != 0) {
				//if there is a new line, increment new line
				if (input[i] == '\n') {
					++newLines;
				}
				//if there is an ending '
				if (input[i] == '\'' && input[i + 1] != '\'') {
					return i + 1;
				}
				//if its not an ending and there is a ', increment
				else if (input[i] == '\'') {
					i++;
				}
				i++;
			}
			return -1;
		}
		return 0;
	}

	TokenType type() {
		return TokenType::STRING;
	}
};

class ID : public Automaton {
public:
	ID() : Automaton(TokenType::ID) {}
	int read(std::string& input) {
		//if the first string is alpha
		if (!input.empty() && isalpha(input.at(0))) {
			int i = 1;
			//while the string is either alpha or numerical
			while (input[i] != 0 && isalnum(input[i])) {
				i++;
			}
			return i;
		}
		return 0;
	}
};

//Class for comment token
class Comment : public Automaton {
public:
	Comment() : Automaton(TokenType::COMMENT) {}
	int read(std::string& input) {
		newLines = 0;
		//if it starts with a #
		if (!input.empty() && input.at(0) == '#') {
			int i = 1;
			//if its not a multiline
			if (input[1] != '|') {
				while (!(input[i] == '\0' || input[i] == '\n')) {
					i++;
				}
				return i;
			}
			//it is a multiline
			else {
				//while it does not have ending sequence
				while (!(input[i] == '|' && input[i + 1] == '#')) {
					if (input[i] == '\n') { ++newLines; }
					i++;
					if (i == (int)input.size()) {
						return -1;
					}
				}
				return i + 2;
			}
		}
		return 0;
	}
};

//Class for Undefined Token
class Undefined : public Automaton {
public:
	Undefined() : Automaton(TokenType::UNDEFINED) {}
	int read(std::string& input) {
		int i = 0;
		newLines = 0;
		//Non terminating comment
		if (input[i] == '#' && input[i + 1] == '|') {
			i = i + 2;
			//while not eof
			while (i < (int)input.size()) {
				//if there is a new line
				if (input[i] == '\n') {
					++newLines;
				}
				//if there is the terminating characters
				if (input[i] == '|' && input[i + 1] == '#') {
					return 0;
				}
				i++;
			}
			return i;
		}
		//Non terminating string
		if (input[i] == '\'') {
			//empty string
			if (input[i + 1] == '\'') {
				return 0;
			}
			i++;
			while (i < (int)input.size()) {
				//apostrophe
				if (input[i] == '\'' && input[i + 1] == '\'') {
					i = i + 1;
				}
				//if there is a terminating char
				else if (input[i] == '\'') {
					return 0;
				}
				if (input[i] == '\n') {
					++newLines;
				}
				i++;
			}
			return i;
		}
		return 0;
	}
};

#endif
