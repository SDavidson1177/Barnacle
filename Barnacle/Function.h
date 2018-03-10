#pragma once
#include <vector>
#include <string>
#include "Block.h"
#include "Statement.h"

class Function;

std::string findVariable(std::vector<std::vector <Assignment>*> variables, std::string p_key);
std::string call(std::vector <Block>** blocks, std::vector<std::vector <Assignment>*> variables, std::vector <Function>* functions,
	int* block_count, int* parameterNum, Function** executedFunction);

class Function {
	bool callingFunction = false;
	int parameterNum = 0;
	Function* executedFunction = nullptr;
public:
	std::string return_value = "no_return";
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

	void evaluate(std::vector <std::vector <Assignment>*> p_variables, std::vector <Function>* functions) {
		bool returning = true;
		parameterNum = 0;
		return_value = "no_return";
		Token last_token("", "");
		const int size = variables.size();
		std::vector <std::vector <Assignment>*> tempVec = {&variables};
		for (int i = 0; i < p_variables.size(); i++) {
			tempVec.push_back(p_variables.at(i));
		}
		evaluateBlocks(&blocks, tempVec, functions, &callingFunction, &parameterNum, &executedFunction, 
				&return_value, &returning, &last_token, "");
	}
};


