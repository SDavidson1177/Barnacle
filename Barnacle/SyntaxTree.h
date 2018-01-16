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
public:
	SyntaxTree();

	SyntaxTree(Lexer lexed_info);

	~SyntaxTree();

	void getBlocks(Lexer* lexed_info);

	void getStatements(std::vector <Assignment>* variables, std::vector <Function>* functions);

	//void start();

	void coutTree();
};
