#include "Parser.h"

void Parser::dataLogProgram() {
	try {
		match(TokenType::SCHEMES);
		match(TokenType::COLON);
		scheme();
		schemeList();
		match(TokenType::FACTS);
		match(TokenType::COLON);
		factList();
		match(TokenType::RULES);
		match(TokenType::COLON);
		ruleList();
		match(TokenType::QUERIES);
		match(TokenType::COLON);
		query();
		queryList();
		match(TokenType::EOFILE);
		status = "Success!";
		success = true;
	}
	catch (std::exception& e) {
		std::stringstream s;
		s << "Failure!" << std::endl;
		s << "  " << e.what();
		status = s.str();
		success = false;
	}
}
void Parser::schemeList() {
	if (next() == TokenType::ID) {
		scheme();
		schemeList();
	}
	else {
		return;
	}
}
void Parser::factList() {
	if (next() == TokenType::ID) {
		fact();
		factList();
	}
	else {
		return;
	}
}
void Parser::ruleList() {
	if (next() == TokenType::ID) {
		rule();
		ruleList();
	}
	else {
		return;
	}
}
void Parser::queryList() {
	if (next() == TokenType::ID) {
		query();
		queryList();
	}
	else {
		return;
	}
}
void Parser::scheme() {
	Predicate* predicate = new Predicate;
	predicate->addHead(matchString(TokenType::ID));
	match(TokenType::LEFT_PAREN);
	predicate->addParameter(new Identifier(matchString(TokenType::ID)));
	idList(predicate);
	match(TokenType::RIGHT_PAREN);
	schemes.push_back(predicate);
	return;
}
void Parser::fact() {
	Predicate* predicate = new Predicate;
	predicate->addHead(matchString(TokenType::ID));
	match(TokenType::LEFT_PAREN);
	std::string in = matchString(TokenType::STRING);
	predicate->addParameter(new String(in));
	in.erase(std::remove(in.begin(), in.end(), '\''), in.end());
	domain.insert(in);
	stringList(predicate);
	match(TokenType::RIGHT_PAREN);
	match(TokenType::PERIOD);
	facts.push_back(predicate);
	return;
}
void Parser::rule() {
	Rule* rule = new Rule;
	rule->addHead(headPredicate());
	match(TokenType::COLON_DASH);
	rule->addPredicate(predicate());
	predicateList(rule);
	match(TokenType::PERIOD);
	rules.push_back(rule);
	return;
}
void Parser::query() {
	queries.push_back(predicate());
	match(TokenType::Q_MARK);
	return;
}
Predicate* Parser::headPredicate() {
	Predicate* p = new Predicate();
	p->addHead(matchString(TokenType::ID));
	match(TokenType::LEFT_PAREN);
	p->addParameter(new Identifier(matchString(TokenType::ID)));
	idList(p);
	match(TokenType::RIGHT_PAREN);
	return p;
}
Predicate* Parser::predicate() {
	Predicate* p = new Predicate;
	p->addHead(matchString(TokenType::ID));
	match(TokenType::LEFT_PAREN);
	p->addParameter(parameter());
	parameterList(p);
	match(TokenType::RIGHT_PAREN);
	return p;
}
void Parser::predicateList(Rule* r) {
	if (next() == TokenType::COMMA) {
		match(TokenType::COMMA);
		r->addPredicate(predicate());
		predicateList(r);
	}
	else {
		return;
	}
}
void Parser::parameterList(Predicate* p) {
	if (next() == TokenType::COMMA) {
		match(TokenType::COMMA);
		p->addParameter(parameter());
		parameterList(p);
	}
	else {
		return;
	}
}
void Parser::stringList(Predicate* p) {
	if (next() == TokenType::COMMA) {
		match(TokenType::COMMA);
		std::string in = matchString(TokenType::STRING);
		p->addParameter(new String(in));
		in.erase(std::remove(in.begin(), in.end(), '\''), in.end());
		domain.insert(in);
		stringList(p);
	}
	else {
		return;
	}
}
void Parser::idList(Predicate* p) {
	if (next() == TokenType::COMMA) {
		match(TokenType::COMMA);
		p->addParameter(new Identifier(matchString(TokenType::ID)));
		idList(p);
	}
	else {
		return;
	}
}
Parameter* Parser::parameter() {
	if (next() == TokenType::STRING) {
		return (new String(matchString(TokenType::STRING)));
	}
	else if (next() == TokenType::ID) {
		return (new Identifier(matchString(TokenType::ID)));
	}
	else if (next() == TokenType::LEFT_PAREN) {
		return expression();
	}
	else {
		return new String("");
	}
}
Expression* Parser::expression() {
	Expression* exp = new Expression;
	match(TokenType::LEFT_PAREN);
	exp->addLeft(parameter());
	exp->addOp(myOperator());
	exp->addRight(parameter());
	match(TokenType::RIGHT_PAREN);
	return exp;
}
std::string Parser::myOperator() {
	if (next() == TokenType::ADD) {
		match(TokenType::ADD);
		return "+";
	}
	match(TokenType::MULTIPLY);
	return "*";
}

std::string Parser::tokenString(TokenType type) {
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