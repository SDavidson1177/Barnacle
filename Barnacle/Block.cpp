#include <vector>
#include <iostream>
#include <new>
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

using namespace std;

Function* findFunction(std::vector <Function>* functions, std::string p_key){
	try {
		for (int i = 0; i < functions->size() + 1; i++) {
			if (i == functions->size()) {
				throw "Error: function undefined";
			}
			else if(stringEquals(p_key, functions->at(i).key)){
				return &functions->at(i);
			}
		}
	}
	catch (const char* error) {
		return nullptr;
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

std::string checkInequality(std::vector <Block>* blocks, std::vector<std::vector <Assignment>*> variables,
	std::vector <Function>* functions, int* block_count, int* parameterNum, int starting_token, int start_block, 
	int last_block) {
	try {
		std::vector <Token> tokens;
		string last_func_value = "";
		int beg_of_exp = *block_count;
		if (last_block < 0) {
			while (*block_count < blocks->size() && blocks->at(*block_count).statement.at(0).getTokenValue() != "Semi-Colon") {
				(*block_count)++;
			}
		}
		else {
			*block_count = last_block;
			beg_of_exp = start_block;
		}
		for (int i = *block_count; i >= beg_of_exp; i--) {
			const int size = blocks->at(i).statement.size();
			if (blocks->at(i).statement.size() == 1) {
				if (blocks->at(i).statement.at(0).getTokenValue() == "RBracket") {
					//Backtrack to determine if we are dealing with a function call or simply order of operations
					do {
						i--;
					} while (i >= 0 && blocks->at(i).statement.at(0).getTokenValue() != "LBracket");
					i--;
					//check for function call
					if (i >= 0 && blocks->at(i).statement.at(blocks->at(i).statement.size() - 1).getTokenValue() == "Symbol") {
						Function* executedFunction = findFunction(functions, blocks->at(i).statement.at(blocks->at(i).statement.size() - 1).getTokenKey());
						if (executedFunction != nullptr) {
							int start = ++i;
							last_func_value = call(&blocks, variables, functions, &start, parameterNum, &executedFunction);
						}
						else {
							cout << "not finding function\n";
							//error, cannot have symbol right before parantheses if it is not a function identifier
						}
					}
					else {
						// not a function call or error with no opening bracket
					}
				}
				else if (blocks->at(i).statement.at(0).getTokenValue() == "Symbol") {
					if (findFunction(functions, blocks->at(i).statement.at(0).getTokenKey()) != nullptr) {
						tokens = pushFront(Token(last_func_value, "Integer"), &tokens);
					}
					else{
						tokens = pushFront(blocks->at(i).statement.at(0), &tokens);
					}
				}
			}
			else {
				int j = (i == last_block) ? starting_token : 0;
				for (; j < size; j++) {
					if (blocks->at(i).statement.at(size - 1 - j).getTokenValue() == "Integer") {
						tokens = pushFront(blocks->at(i).statement.at(size - 1 - j), &tokens);
					}
					else if (blocks->at(i).statement.at(size - 1 - j).getTokenValue() == "Addition" || blocks->at(i).statement.at(size - 1 - j).getTokenValue() == "Subtraction"
						|| blocks->at(i).statement.at(size - 1 - j).getTokenValue() == "Multiplication" || blocks->at(i).statement.at(size - 1 - j).getTokenValue() == "Division"
						|| blocks->at(i).statement.at(size - 1 - j).getTokenValue() == "False"
						|| blocks->at(i).statement.at(size - 1 - j).getTokenValue() == "True") {
						tokens = pushFront(blocks->at(i).statement.at(size - 1 - j), &tokens);
					}
					else if (blocks->at(i).statement.at(size - 1 - j).getTokenValue() == "Symbol") { // Change so that it determines what to do if it encounters an identifier
						if (findFunction(functions, blocks->at(i).statement.at(size - 1 - j).getTokenKey()) != nullptr) {
							tokens = pushFront(Token(last_func_value, "Integer"), &tokens);
						}
						else {
							tokens = pushFront(blocks->at(i).statement.at(size - 1 - j), &tokens);
						}
					}
					else if (blocks->at(i).statement.at(size - 1 - j).getTokenValue() == "Equality") {
						BinaryOperation op = BinaryOperation(&tokens);
						while (!op.execute(variables, 0)) {
						}
						cout << "Equality: " << to_string(op.execute(variables)) << " == " << checkInequality(blocks, variables, functions, block_count, parameterNum, j + 1, beg_of_exp, i) << endl;
						//cout << "In Block: " << i - 1 << " Giving J: " << j + 1 << " End at: " << beg_of_exp << endl;
						if (to_string(op.execute(variables)) == checkInequality(blocks, variables, functions, block_count, parameterNum, j + 1, beg_of_exp, i)) {
							return "true";
						}
						return "false";
					}
					else if (blocks->at(i).statement.at(size - 1 - j).getTokenValue() == "Assignment") {
						break; // Once we reach an assignment operator stop. (Since the expression is in the latter part)
					}
					else {
						throw "invalid expression";
					}
				}
			}
		}
		BinaryOperation op = BinaryOperation(&tokens);
		while (!op.execute(variables, 0)) {
		}
		return std::to_string(op.execute(variables));
	}
	catch (const char* error) {
		cout << "Error: " << error;
		return "BARNACLE_COMP_ERROR";
	}
}

void evaluateBlocks(std::vector <Block>* blocks, std::vector<std::vector <Assignment>*> variables, std::vector <Function>* functions, bool* callingFunction,
	int* parameterNum, Function** executedFunction, std::string* p_return_value, bool* returning,
	Token* last_token, std::string function_key) {
	try {
		for (int block_count = 0; block_count < blocks->size(); block_count++) {
			if (inFunction) {
				if (blocks->at(block_count).statement.at(0).getTokenValue() == "LBracket" && !inBracket && !inBrace) {
					inBracket = true;
				}
				else if (blocks->at(block_count).statement.at(0).getTokenValue() == "RBracket" && inBracket && !inBrace) {
					inBracket = false;
				}
				else if (blocks->at(block_count).statement.at(0).getTokenValue() == "LBrace" && !inBracket && !inBrace) {
					inBrace = true;
				}
				else if (blocks->at(block_count).statement.at(0).getTokenValue() == "RBrace" && !inBracket && inBrace) {
					inBrace = false;
					inFunction = false;
				}
				else if (inBracket) {
					if (blocks->at(block_count).statement.size() == 1 && blocks->at(block_count).statement.at(0).getTokenValue() == "Symbol") {
						functions->at(functions->size() - 1).variables.push_back(Assignment(blocks->at(block_count).statement.at(0).getTokenKey(), ""));
					}
					else {
						throw "Error invalid parameters";
					}
				}
				else if (inBrace) {
					functions->at(functions->size() - 1).appendBlock(&(blocks->at(block_count)));
				}
				else {
					throw "Error creating function";
				}
			}
			else if (blocks->at(block_count).statement.size() == 1) {
				if (blocks->at(block_count).statement.at(0).getTokenValue() == "Symbol") {
					//make a call
					//temperary
					bool found = false;
					int i = 0;
					for (int indexOfVec = 0; indexOfVec < variables.size(); indexOfVec++) {
						i = 0;
						while (i < variables.at(indexOfVec)->size()) {
							if (stringEquals(blocks->at(block_count).statement.at(0).getTokenKey(), variables.at(indexOfVec)->at(i).key)) {
								std::cout << variables.at(indexOfVec)->at(i).value;
								found = true;
								indexOfVec = variables.size();
								break;
							}
							i++;
						}
					}
					i = 0;
					if (!found) {
						while (i < functions->size()) {
							if (stringEquals(blocks->at(block_count).statement.at(0).getTokenKey(), functions->at(i).key)) {
								found = true;
								*executedFunction = &functions->at(i);
								checkInequality(blocks, variables, functions, &block_count, parameterNum);
								break;
							}
							i++;
						}
					}
					if (!found) {
						throw "Error undeclared identifier";
					}
				}
				else if (blocks->at(block_count).statement.at(0).getTokenValue() == "Semi-Colon") {
					/*if (*p_return_value != "no_return") {
						std::cout << *p_return_value;
						*p_return_value = "no_return";
					}*/
				}
				else {
					throw "invalid_call";
				}
			}
			else if (blocks->at(block_count).statement.size() == 2) { // For things such as Function and Class declarations.
				if (blocks->at(block_count).statement.at(0).getTokenValue() == "Function_Dec" && blocks->at(block_count).statement.at(1).getTokenValue() == "Symbol") {
					functions->push_back(Function(blocks->at(block_count).statement.at(1).getTokenKey()));
					inFunction = true;
				}
				else if (blocks->at(block_count).statement.at(0).getTokenKey() == "return") {
					std::string retval = findVariable(variables, blocks->at(block_count).statement.at(1).getTokenKey());
					if (retval != "NULL") {
						*p_return_value = retval;
						block_count = blocks->size();
					}
					else {
						throw "invalid return";
					}
				}
				else if (blocks->at(block_count).statement.at(0).getTokenValue() == "Var_Dec" && blocks->at(block_count).statement.at(1).getTokenValue() == "Symbol") {
					for (int indexOfVec = 0; indexOfVec < variables.size(); indexOfVec++) {
						for (int i = 0; i < variables.at(indexOfVec)->size() + 1; i++) {
							if (i == variables.at(indexOfVec)->size()) {
								variables.at(indexOfVec)->push_back(Assignment(blocks->at(block_count).statement.at(1).getTokenKey(), ""));
								break;
							}
							else if (blocks->at(block_count).statement.at(1).getTokenKey() == variables.at(indexOfVec)->at(i).key) {
								variables.at(indexOfVec)->at(i) = Assignment(blocks->at(block_count).statement.at(1).getTokenKey(), "");
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
			else if (blocks->at(block_count).statement.size() > 2 && blocks->at(block_count).statement.at(0).getTokenValue() == "Var_Dec") {
				if (blocks->at(block_count).statement.at(2).getTokenValue() == "Assignment") {
					std::string key = blocks->at(block_count).statement.at(1).getTokenKey();
					vector <Token> temp_statement = blocks->at(block_count).statement;
					temp_statement.erase(temp_statement.begin(), temp_statement.begin() + 3);
					BinaryOperation op(&temp_statement);
					while (!op.execute(variables, 0)) {
					}
					for (int i = 0; i < variables.at(0)->size() + 1; i++) {
						if (i == variables.at(0)->size()) {
							variables.at(0)->push_back(Assignment(key, checkInequality(blocks, variables, functions, &block_count, parameterNum)));
							break;
						}
						else if (key == variables.at(0)->at(i).key) {
							//cout << "getting variable...\n";
							variables.at(0)->at(i).value = checkInequality(blocks, variables, functions, &block_count, parameterNum);
							break;
						}
					}
				}
			}
			else if (blocks->at(block_count).statement.size() > 2) {
				std::string function_key = "";
				std::vector <Token> tokens;
				std::string val = checkInequality(blocks, variables, functions, &block_count, parameterNum);
				std::cout << val;
			}
		}
	}
	catch(const char* error){
		std::cout << "An error occured: " << error;
	}
}