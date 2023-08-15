#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType : int {
	ERROR,
	COMMA,
	PERIOD,
	Q_MARK,
	LEFT_PAREN,
	RIGHT_PAREN,
	COLON,
	COLON_DASH,
	MULTIPLY,
	ADD,
	SCHEMES,
	FACTS,
	RULES,
	QUERIES,
	ID,
	STRING,
	COMMENT,
	UNDEFINED,
	EOFILE
};



class Token {
private:
	TokenType type;
	std::string description;
	int line;
public:
	Token() {
		type = TokenType::ERROR;
		description = "";
		line = -1;
	}

	Token(TokenType initType, std::string myDescription, int line) : type(initType), description(myDescription), line(line) {}

	void setType(TokenType input) {
		type = input;
	}
	void setDescription(std::string desc) {
		description = desc;
	}
	void setLine(int input) {
		line = input;
	}

	std::string get() {
		return description;
	}

	TokenType getType() {
		return type;
	}

	std::string typeString() {
		switch (type) {
		case TokenType::COMMA: return "COMMA"; break;
		case TokenType::PERIOD: return "PERIOD"; break;
		case TokenType::Q_MARK: return "Q_MARK"; break;
		case TokenType::LEFT_PAREN: return "LEFT_PAREN"; break;
		case TokenType::RIGHT_PAREN: return "RIGHT_PAREN"; break;
		case TokenType::COLON: return "COLON"; break;
		case TokenType::COLON_DASH: return "COLON_DASH"; break;
		case TokenType::MULTIPLY: return "MULTIPLY"; break;
		case TokenType::ADD: return "ADD"; break;
		case TokenType::SCHEMES: return "SCHEMES"; break;
		case TokenType::FACTS: return "FACTS"; break;
		case TokenType::RULES: return "RULES"; break;
		case TokenType::QUERIES: return "QUERIES"; break;
		case TokenType::ID: return "ID"; break;
		case TokenType::STRING: return "STRING"; break;
		case TokenType::COMMENT: return "COMMENT"; break;
		case TokenType::UNDEFINED: return "UNDEFINED"; break;
		case TokenType::EOFILE: return "EOF"; break;
		case TokenType::ERROR: return "ERROR";
		}
		return "ERROR";
	}
	std::string toString() {
		return "(" + typeString() + ",\"" + description + "\"," + std::to_string(line) + ")";
	}
};

#endif
