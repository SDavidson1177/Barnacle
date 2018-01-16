#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <new>
#include "Lexer.h"
#include "SyntaxTree.h"

using namespace std;

// Important global variables
string master_user_input;
std::vector <Assignment> variables; //keeps track of all of the variables
std::vector <Function> functions;

void evaluate(string);
void getInput() {
	getline(cin, master_user_input);
	Lexer test = Lexer(master_user_input);
	test.getTokens();
	//cout << test_quote[0][0] << endl;
	if (master_user_input != "exit") {
		getInput();
	}
}

int main(int argc, char** argv) {
	Lexer lexed_file;
	SyntaxTree main_syntax_tree;
	fstream file;
	for (int arguments = 0; arguments < argc; arguments++) {
		file.open(argv[arguments]);
		if (file.is_open()) {
			while (getline(file, master_user_input)) {
				lexed_file.parse(master_user_input);
				//cout << master_user_input << endl;
			}
		}
	}
	main_syntax_tree.getBlocks(&lexed_file);
	//main_syntax_tree.coutTree();
	main_syntax_tree.getStatements(&variables, &functions);
	//testBinOp.makeOperation(&lexed_file.tokens, 0);
	//testBinOp.execute(&variables, 0);
	//cout << testBinOp.execute(&variables);
	//lexed_file.getTokens();
	return 0;
}

void evaluate(string exp, int result) {

}

