#pragma once
#include <vector>
#include <string>
#include "Block.h"
#include "Statement.h"

class Function {
public:
	std::string key;
	int numOfParameters;
	std::vector <Assignment> variables;
	std::vector <Block> blocks;
	Function();
	Function(std::string p_key);
	~Function();
	void addVariable(std::string* p_key);
	void appendBlock(Block* bl);
	void evaluate(std::vector <std::vector <Assignment>*> variables, std::vector <Function>* functions);
};

class Call {
	Function function;
	
	std::vector <Block> blocks;
public:
	void appendBlock(Block* bl);
	void
};
