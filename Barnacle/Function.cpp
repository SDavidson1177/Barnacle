#include <iostream>
#include <string>
#include "Function.h"
#include "Statement.h"
//#include "ToolFunctions.h"

using namespace std;

// Memebers for the function class
Function::Function() {
	this->key = "";
	this->numOfParameters = 0;
}

Function::Function(std::string p_key) {
	this->key = p_key;
	this->numOfParameters = 0;
}

Function::~Function() {
	delete executedFunction;
}

void Function::appendBlock(Block* bl) {
	this->blocks.push_back(*bl);
}

void Function::coutBlocks() {
	for (int i = 0; i < this->blocks.size(); i++) {
		std::cout << "Block " << i << std::endl;
		this->blocks.at(i).coutBlock();
		std::cout<< "End of Block...\n";
	}
}

void Function::addVariable(std::string* p_key) {
	for (int i = 0; i <this->variables.size() + 1; i++) {
		if (i == this->variables.size()) {
			this->variables.push_back(Assignment(*p_key, ""));
			break;
		}
		else if (*p_key == this->variables.at(i).key) {
			this->variables.at(i) = Assignment(*p_key, "");
			break;
		}
	}
	this->numOfParameters++;
}

// Function for finding the value of a given variable
std::string findVariable(std::vector<std::vector <Assignment>*> variables, std::string p_key) {
	try {
		for (int indexOfVec = 0; indexOfVec < variables.size() + 1; indexOfVec++) {
			if (indexOfVec == variables.size()) {
				throw "Error variable as parameter undefined";
			}
			else {
				for (int i = 0; i < variables.at(indexOfVec)->size(); i++) {
					if (p_key == variables.at(indexOfVec)->at(i).key) {
						return variables.at(indexOfVec)->at(i).value;
					}
				}
			}
		}
	}
	catch (const char* error) {
		return "NULL";
	}
}

std::string call(std::vector <Block>** blocks, std::vector<std::vector <Assignment>*> variables, std::vector <Function>* functions,
	int* block_count,int* parameterNum, Function** executedFunction) {
	while (*block_count < (*blocks)->size() && (*blocks)->at(*block_count).statement.at(0).getTokenValue() != "RBracket") {
		if ((*blocks)->at(*block_count).statement.size() > 2) {
			std::vector <Token> tokens;
			(*executedFunction)->variables.at(*parameterNum).value = checkInequality(*blocks, variables, functions, block_count, parameterNum);
			*parameterNum = *parameterNum + 1;
		}
		else if ((*blocks)->at(*block_count).statement.at(0).getTokenValue() == "Integer") {
			(*executedFunction)->variables.at(*parameterNum).value = (*blocks)->at(*block_count).statement.at(0).getTokenKey();
			*parameterNum = *parameterNum + 1;
		}
		else if ((*blocks)->at(*block_count).statement.at(0).getTokenValue() == "Symbol") {
			(*executedFunction)->variables.at(*parameterNum).value = findVariable(variables, (*blocks)->at(*block_count).statement.at(0).getTokenKey());
			cout << "param value..." << (*executedFunction)->variables.at(*parameterNum).value << endl;
			if ((*executedFunction)->variables.at(*parameterNum).value == "NULL") {
				throw "undeclared variable as function parameter";
			}
			*parameterNum = *parameterNum + 1;
		}
		else if ((*blocks)->at(*block_count).statement.at(0).getTokenValue() == "RBracket") {
			break;
		}
		(*block_count)++;
	}
	const int var_size = (*executedFunction)->variables.size();
	(*executedFunction)->evaluate(variables, functions);
	*parameterNum = 0;
	std::string retval = (*executedFunction)->return_value;
	const int size = (*executedFunction)->variables.size();
	/*for (int i = 0; i < size; i++) {
		(*executedFunction)->variables.at(i).value = "";
	}*/
	executedFunction = nullptr;
	//cout << "return value..." << retval << endl;
	return retval;
}