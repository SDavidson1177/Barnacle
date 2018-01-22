#pragma once
#include <vector>
#include <string>
#include "Block.h"
#include "Statement.h"

class Function;

class Function {
	bool callingFunction = false;
	int parameterNum = 0;
	Function* executedFunction = nullptr;
	std::string return_value = "no_return";
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
	void coutBlocks();

	std::string operator= (std::string& p_key) {
		return return_value;
	}

	void evaluate(std::vector <std::vector <Assignment>*> variables, std::vector <Function>* functions) {
		bool returning = true;
		std::vector <std::vector <Assignment>*> tempVec = {&this->variables};
		for (int i = 0; i < variables.size(); i++) {
			tempVec.push_back(variables.at(i));
		}
		for (int i = 0; i < this->blocks.size(); i++) {
			blocks.at(i).evaluate(tempVec, functions, &callingFunction, &parameterNum, &executedFunction, 
				&return_value, &returning);
		}
	}
};

class Call {
	Function function;
	
	std::vector <Block> blocks;
public:
	void appendBlock(Block* bl);
};
