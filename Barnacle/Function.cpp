#include <iostream>
#include <string>
#include "Function.h"
#include "Statement.h"
//#include "ToolFunctions.h"

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