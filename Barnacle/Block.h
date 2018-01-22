#pragma once
#include <vector>
#include "Token.h"
#include "Statement.h"
/*A Block is a group of tokens that when arranged together
form a statement. A statement is a single action of the programming
language that will be later defined. Methods to determine the
semantics of the block will later be used to evaluate whether there
is logical meaning or not to the block.*/

class Function;

class Block {
	std::vector <Token> statement;

public:
	Block();

	~Block();

	void push_back(Token tokn);

	bool isEmpty();

	void coutBlock();

	int size();

	std::string checkInequality(std::vector<std::vector <Assignment>*> variables);

	void evaluate(std::vector<std::vector <Assignment>*> variables, std::vector <Function>* functions, 
		bool* callingFunction, int* parameterNum, Function** executedFunction, std::string* p_return_value = nullptr,
		bool* returning = nullptr);
};