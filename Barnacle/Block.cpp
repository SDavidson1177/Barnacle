#include <vector>
#include <iostream>
#include "Block.h"
#include "Token.h"
#include "Statement.h"
#include "Function.h"
#include "ToolFunctions.h"

//Global variables for evaluating in functions and what not
bool inFunction = false;
bool bracketNeeded = false;
bool inBracket = false;
bool braceNeeded = false;
bool inBrace = false;

bool callingFunction = false;
int funcID = -1;
int parameterNum = 0;
Function executedFunction;

// Function for finding the value of a given variable
std::string findVariable(std::vector<std::vector <Assignment>*> variables, std::string p_key) {
	for (int indexOfVec = 0; indexOfVec < variables.size() + 1; indexOfVec++) {
		if (indexOfVec == variables.size()) {
			return "Error variable as parameter undefined";
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

//Global variables for evaluating if statements

Block::Block() {

}

Block::~Block() {

}

void Block::push_back(Token tokn) {
	this->statement.push_back(tokn);
}

bool Block::isEmpty() {
	return this->statement.empty();
}

void Block::coutBlock() {
	for (int token_index = 0; token_index < this->statement.size(); token_index++) {
		this->statement.at(token_index).coutToken();
	}
}

int Block::size() {
	return this->statement.size();
}

std::string Block::checkInequality(std::vector<std::vector <Assignment>*> variables) {
	std::vector <Token> tokens;
	const int size = this->statement.size();
	for (int i = 0; i < size; i++) {
		if (this->statement.at(size - 1 - i).getTokenValue() == "Integer") {
			tokens = pushFront(this->statement.at(size - 1 - i), &tokens);
		}else if(this->statement.at(size - 1 - i).getTokenValue() == "Addition" || this->statement.at(size - 1 - i).getTokenValue() == "Subtraction"
			|| this->statement.at(size - 1 - i).getTokenValue() == "Multiplication" || this->statement.at(size - 1 - i).getTokenValue() == "Division"
			|| this->statement.at(size - 1 - i).getTokenValue() == "Symbol" || this->statement.at(size - 1 - i).getTokenValue() == "False"
			|| this->statement.at(size - 1 - i).getTokenValue() == "True"){
			tokens = pushFront(this->statement.at(size - 1 - i), &tokens);
		}
		else if (this->statement.at(size - 1 - i).getTokenValue() == "Equality") {
			BinaryOperation op = BinaryOperation(&tokens);
			while (!op.execute(variables, 0)) {
			}
			this->statement.erase(this->statement.end() - i - 1, this->statement.end());
			if (op.execute(variables) == (float)toNum(this->checkInequality(variables))) {
				return "true";
			}else{
				return "false";
			}
			break;
		}
		else {
			return "error";
		}
	}
	BinaryOperation op = BinaryOperation(&tokens);
	while (!op.execute(variables, 0)) {
	}
	return std::to_string(op.execute(variables));
}

void Block::evaluate(std::vector<std::vector <Assignment>*> variables, std::vector <Function>* functions) {
	try {
		if (inFunction) {
			if (this->statement.at(0).getTokenValue() == "LBracket" && !inBracket && !inBrace) {
				inBracket = true;
			}
			else if (this->statement.at(0).getTokenValue() == "RBracket" && inBracket && !inBrace) {
				inBracket = false;
			}
			else if (this->statement.at(0).getTokenValue() == "LBrace" && !inBracket && !inBrace) {
				inBrace = true;
			}
			else if (this->statement.at(0).getTokenValue() == "RBrace" && !inBracket && inBrace) {
				inBrace = false;
				inFunction = false;
			}
			else if (inBracket) {
				if (this->statement.size() == 1 && this->statement.at(0).getTokenValue() == "Symbol") {
					functions->at(functions->size() - 1).variables.push_back(Assignment(this->statement.at(0).getTokenKey(), ""));
				}
				else {
					throw "Error invalid parameters";
				}
			}
			else if (inBrace) {
				functions->at(functions->size() - 1).appendBlock(this);
			}
			else {
				throw "Error creating function";
			}
		}else if(callingFunction){
			if (this->statement.at(0).getTokenValue() == "LBracket" && !inBracket) {
				inBracket = true;
			}
			else if (this->statement.at(0).getTokenValue() == "RBracket" && inBracket) {
				inBracket = false;
				callingFunction = false;
				executedFunction.evaluate(variables, functions);
			}else if(inBracket){
				if (parameterNum >= executedFunction.variables.size()) {
					throw "Error too many parameter values in function call";
				}
				else {
					if (this->statement.at(0).getTokenValue() == "Symbol") {
						for (int indexOfVec = 0; indexOfVec < variables.size() + 1; indexOfVec++) {
							if (indexOfVec == variables.size()) {
								throw "Error variable as parameter undefined";
							}
							else {
								for (int i = 0; i < variables.at(indexOfVec)->size(); i++) {
									if (this->statement.at(0).getTokenKey() == variables.at(indexOfVec)->at(i).key) {
										executedFunction.variables.at(parameterNum).value = variables.at(indexOfVec)->at(i).value;
										indexOfVec = variables.size() + 1;
										parameterNum++;
										break;
									}
								}
							}
						}
					}
					else if(this->statement.at(0).getTokenValue() == "Integer"){
						executedFunction.variables.at(parameterNum).value = this->statement.at(0).getTokenKey();
						parameterNum++;
					}
					else {
						throw "Error: invalid parameter type";
					}
				}
			}else{
				throw "Error: invalid function call";
			}
		}
		else if (this->statement.size() < 2) {
			if (this->statement.at(0).getTokenValue() == "Symbol") {
				//make a call
				//temperary
				bool found = false;
				int i = 0;
				for (int indexOfVec = 0; indexOfVec < variables.size(); indexOfVec++) {
					i = 0;
					while (i < variables.at(indexOfVec)->size()) {
						if (stringEquals(this->statement.at(0).getTokenKey(), variables.at(indexOfVec)->at(i).key)) {
							std::cout << variables.at(indexOfVec)->at(i).value;
							found = true;
							indexOfVec = variables.size();
							break;
						}
						i++;
					}
				}
				if (!found) {
					i = 0;
					while (i < functions->size()) {
						if (stringEquals(this->statement.at(0).getTokenKey(), functions->at(i).key)) {
							//functions->at(i).evaluate(variables, functions);
							funcID = i;
							parameterNum = 0;
							callingFunction = true;
							executedFunction = functions->at(i);
							found = true;
							break;
						}
						i++;
					}
				}

				if (!found) {
					throw "Error undeclared identifier";
				}
			}
			else {
				throw "invalid_call";
			}
		}else if(this->statement.size() == 2){ // For things such as Function and Class declarations.
			if (this->statement.at(0).getTokenValue() == "Function_Dec" && this->statement.at(1).getTokenValue() == "Symbol") {
				functions->push_back(Function(this->statement.at(1).getTokenKey()));
				inFunction = true;
			}
			else if (this->statement.at(0).getTokenValue() == "Var_Dec" && this->statement.at(1).getTokenValue() == "Symbol") {
				for (int indexOfVec = 0; indexOfVec < variables.size(); indexOfVec++) {
					for (int i = 0; i < variables.at(indexOfVec)->size() + 1; i++) {
						if (i == variables.at(indexOfVec)->size()) {
							variables.at(indexOfVec)->push_back(Assignment(this->statement.at(1).getTokenKey(), ""));
							break;
						}
						else if (this->statement.at(1).getTokenKey() == variables.at(indexOfVec)->at(i).key) {
							variables.at(indexOfVec)->at(i) = Assignment(this->statement.at(1).getTokenKey(), "");
							indexOfVec = variables.size();
							break;
						}
					}
				}
			}
			else {
				throw "invalid_block";
			}
		}
		else if (this->statement.size() > 2 && this->statement.at(0).getTokenValue() == "Var_Dec") {
			if (this->statement.at(2).getTokenValue() == "Assignment") {
				std::string key = this->statement.at(1).getTokenKey();
				this->statement.erase(this->statement.begin(), this->statement.begin() + 3);
				BinaryOperation op(&this->statement);
				while (!op.execute(variables, 0)) {
				}
				for (int i = 0; i < variables.at(0)->size() + 1; i++) {
					if (i == variables.at(0)->size()) {
						variables.at(0)->push_back(Assignment(key, std::to_string(op.execute(variables))));
						break;
					}else if(key == variables.at(0)->at(i).key){
						variables.at(0)->at(i) = Assignment(key, std::to_string(op.execute(variables)));
						break;
					}
				}
			}
		}
		else if (this->statement.size() > 2) {
			std::cout << this->checkInequality(variables);
		}
	}
	catch(const char* error){
		std::cout << "An error occured: " << error;
	}
}