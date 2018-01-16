#include <iostream>
#include <string>
#include <new>
#include <exception>
#include "Statement.h"
#include "Token.h"
#include "ToolFunctions.h"

int toNum(std::string);

Statement::Statement() {

}

Statement::~Statement() {

}

// Member functions for the Assignment class

Assignment::Assignment(std::string p_key, std::string p_value) {
	this->key = p_key;
	this->value = p_value;
}

Assignment::~Assignment() {

}

void Assignment::coutAssignment() {
	std::cout << this->key << " = " << this->value << std::endl;
}

BinaryOperation::BinaryOperation(std::vector <Token>* t) {
	this->left = "";
	this->operation = "";
	this->right = nullptr;
	this->makeOperation(t, 0);
}

BinaryOperation::BinaryOperation(std::string p_left, std::string p_operation, BinaryOperation* p_right) {
	this->left = p_left;
	this->operation = p_operation;
	this->right = p_right;
}

BinaryOperation::~BinaryOperation() {
	if (this->right != nullptr) {
		delete this->right;
	}
}

/*Expression::Expression() {
	this->root = nullptr;
}

Expression::~Expression() {
	this->destroyExpression();
}

void Expression::destroyExpression() {
	destroyExpression(this->root);
}

void Expression::destroyExpression(BinaryOperation* BinOp) {
	if (BinOp != nullptr) {
		Expression::destroyExpression(BinOp->right);
		delete BinOp;
	}
}*/

void BinaryOperation::makeOperation(std::vector<Token>* list_of_tokens, int p_op_needed) {
	try {
			                 // p_op_needed checks that the expression goes in the following order
							  // Symbol/Number -> Operation -> (Symbol/Number or Expression)
							  // If for some reason it does not, then it becomes an invalid expression
							  // and the expection invalid_expr is thrown.
		int size = list_of_tokens->size();
		for (int i = 0; i < size; i++) {
			if (p_op_needed == 0) { // we need a Symbol/Number
				if (list_of_tokens->at(0).getTokenValue() == "Symbol"
					|| list_of_tokens->at(0).getTokenValue() == "Integer") {
					this->left = list_of_tokens->at(0).getTokenKey();
					list_of_tokens->erase(list_of_tokens->begin());
					p_op_needed++;
				}
				else if (list_of_tokens->at(0).getTokenValue() == "True") { // Replace true with 1 and false with 0
					this->left = "1";
					list_of_tokens->erase(list_of_tokens->begin());
					p_op_needed++;
				}else if(list_of_tokens->at(0).getTokenValue() == "False"){
					this->left = "0";
					list_of_tokens->erase(list_of_tokens->begin());
					p_op_needed++;
				}
				else {
					throw "invalid_expr";
				}
			}else if (p_op_needed == 1){ // we need an Operation
				this->operation = list_of_tokens->at(0).getTokenKey(); //add Error Checking later
				list_of_tokens->erase(list_of_tokens->begin());
				p_op_needed++;
			}
			else { // we need an Expression
				if (list_of_tokens->at(0).getTokenValue() == "Symbol"
					|| list_of_tokens->at(0).getTokenValue() == "Integer") {
					this->right = new BinaryOperation(list_of_tokens);
					break;
				}
				else {
					throw "invalid_expr";
				}
			}
		}
	}
	catch (const char* error) {
		std::cout << "An error occured " << error;
	}
}

void BinaryOperation::coutBinaryOperation() {
	std::cout << this->left << " " << this->operation << " ";
	if (this->right != nullptr) {
		this->right->coutBinaryOperation();
	}
}

// Recursively executes a binary expression. This only handles + and -.
float BinaryOperation::execute(std::vector <std::vector <Assignment>*> variables) { // No order of operations
	try {
		if (this->right == nullptr) {
			if (isNaN(this->left)) {
				for (int indexOfVec = 0; indexOfVec < variables.size() + 1; indexOfVec++) {
					if (indexOfVec == variables.size()) {
						throw "Error: Variable '" + this->left + "' is not defined";
					}
					else {
						for (int i = 0; i < variables.at(indexOfVec)->size(); i++) {
							if (this->left == variables.at(indexOfVec)->at(i).key) {
								return (float)toNum(variables.at(indexOfVec)->at(i).value);
								indexOfVec = variables.size();
								break;
							}
						}
					}
				}
			}
			else {
				return (float)toNum(this->left);
			}
		}
		else if (this->operation == "+") {
			if (isNaN(this->left)) {
				for (int indexOfVec = 0; indexOfVec < variables.size() + 1; indexOfVec++) {
					if (indexOfVec == variables.size()) {
						throw "Error: Variable '" + this->left + "' undefined";
					}
					else {
						for (int i = 0; i < variables.at(indexOfVec)->size(); i++) {
							if (variables.at(indexOfVec)->at(i).key == this->left) {
								return (float)toNum(variables.at(indexOfVec)->at(i).value) + this->right->execute(variables);
								indexOfVec = variables.size();
								break;
							}
						}
					}
				}
			}
			else {
				return (float)toNum(this->left) + this->right->execute(variables);
			}
		}
		else if (this->operation == "-") {
			if (isNaN(this->left)) {
				for (int indexOfVec = 0; indexOfVec < variables.size(); indexOfVec++) {
					for (int i = 0; i < variables.at(indexOfVec)->size() + 1; i++) {
						if (variables.at(indexOfVec)->at(i).key == this->left) {
							return (float)toNum(variables.at(indexOfVec)->at(i).value) - this->right->execute(variables);
							indexOfVec = variables.size();
							break;
						}
					}
				}
			}
			else {
				return (float)toNum(this->left) - this->right->execute(variables);
			}
		}
		else {
			return -1;
		}
	}
	catch (const char* error) {
		std::cout << error;
	}
	return -1;
}

// Recursively executes a binary expression. This only handles * and /.
bool BinaryOperation::execute(std::vector <std::vector <Assignment>*> variables, int phase) {
	bool done = true;
	try {
		if (this == nullptr || this->right == nullptr) {
		}
		else if (this->operation == "*" || this->operation == "/") {
			done = false;
			float left = (float)toNum(this->left), right_left = (float)toNum(this->right->left);
			if (isNaN(this->left)) {
				for (int indexOfVec = 0; indexOfVec < variables.size() + 1; indexOfVec++) {
					if (indexOfVec == variables.size()) {
						throw "Error: Variable '" + this->left + "' undefined";
					}
					else {
						for (int i = 0; i < variables.at(indexOfVec)->size() + 1; i++) {
							if (this->left == variables.at(indexOfVec)->at(i).key) {
								left = (float)toNum(variables.at(indexOfVec)->at(i).value);
								indexOfVec = variables.size();
								break;
							}
						}
					}
				}
			}
			if (isNaN(this->right->left)) {
				for (int indexOfVec = 0; indexOfVec < variables.size() + 1; indexOfVec++) {
					if (indexOfVec == variables.size()) {
						throw "Error: Variable '" + this->right->left + "' undefined";
					}
					else {
						for (int i = 0; i < variables.at(indexOfVec)->size(); i++) {
							if (this->right->left == variables.at(indexOfVec)->at(i).key) {
								right_left = (float)toNum(variables.at(indexOfVec)->at(i).value);
								indexOfVec = variables.size();
								break;
							}
						}
					}
				}
			}

			if (this->operation == "*") {
				this->left = std::to_string(left * right_left);
			}
			else {
				this->left = std::to_string(left / right_left);
			}
			this->operation = this->right->operation;
			this->right = this->right->right;
			if (this->right != nullptr) {
				this->right->execute(variables, 0);
			}
		}
		else {
			this->right->execute(variables, 0);
		}
	}
	catch (const char* error) {
		std::cout << "variable '" << error << "' undefined.";
	}
	return done;
}

