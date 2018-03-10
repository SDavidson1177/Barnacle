#pragma once
#include <vector>
#include <string>
#include "Token.h"
#include "Statement.h"
/*A Block is a group of tokens that when arranged together
form a statement. A statement is a single action of the programming
language that will be later defined. Methods to determine the
semantics of the block will later be used to evaluate whether there
is logical meaning or not to the block.*/

class Function;

class Block {
public:
	std::vector <Token> statement;

	Block();

	~Block();

	void push_back(Token tokn);

	bool isEmpty();

	void coutBlock();

	int size();

	/*std::string checkInequality(std::vector <Block>* blocks, std::vector<std::vector <Assignment>*> variables,
		std::vector <Function>* functions, int* block_count, int* parameterNum);*/

	/*void evaluate(std::vector<std::vector <Assignment>*> variables, std::vector <Function>* functions, 
		bool* callingFunction, int* parameterNum, Function** executedFunction, std::string* p_return_value = nullptr,
		bool* returning = nullptr, Token* last_token = nullptr, std::string function_key = "");*/
};

std::string checkInequality(std::vector <Block>* blocks, std::vector<std::vector <Assignment>*> variables,
	std::vector <Function>* functions, int* block_count, int* parameterNum, int starting_token = 0, 
	int start_block = 0, int last_block = -1);

void evaluateBlocks(std::vector <Block>* blocks, std::vector<std::vector <Assignment>*> variables, std::vector <Function>* functions, bool* callingFunction,
	int* parameterNum, Function** executedFunction, std::string* p_return_value, bool* returning,
	Token* last_token, std::string function_key);
