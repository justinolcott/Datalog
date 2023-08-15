
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <streambuf>
#include "Lexer.h"
#include "Parser.h"
#include "Database.h"
#include "Interpreter.h"

std::string stringizer(std::string file) {
	std::ifstream f(file);
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

int main(int argc, char* argv[]) {
	Lexer lexer(stringizer(argv[1]));
	Parser parser(lexer.get());
	DatalogProgram data = parser.run();
	Interpreter interpreter(data);
	interpreter.schemes();
	interpreter.facts();
	interpreter.ruleOpt();
	std::cout << std::endl;
	interpreter.query();
	return 0;
}
