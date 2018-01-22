#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Block.h"
#include "Lexer.h"
#include "Statement.h"
#include "Function.h"

/*A SyntaxTree is a combination of blocks that form
the basis for a computable program.*/

class SyntaxTree {
	std::vector <Block> Blocks; // This will be changed later on.
	std::vector <Statement> Statementes;
	bool callingFunction = false;
	int parameterNum = 0;
	Function* executedFunction = nullptr;
public:
	SyntaxTree();

	SyntaxTree(Lexer lexed_info);

	~SyntaxTree();

	void getBlocks(Lexer* lexed_info);

	//void start();

	void coutTree();

	void getStatements(std::vector <Assignment>* variables, std::vector <Function>* functions) {
		for (int i = 0; i < Blocks.size(); i++) {
			Blocks.at(i).evaluate({ variables }, functions, &callingFunction, &parameterNum, &executedFunction);
		}
	}
};
