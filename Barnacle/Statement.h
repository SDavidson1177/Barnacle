#pragma once
#include <string>
#include <vector>
#include "Token.h"



class Statement {
public:

	Statement();

	~Statement();

	//virtual float execute(std::vector <Assignment>* variables) =0;
	//virtual void execute(std::vector <Assignment>* variables,int phase) =0;
};

class Assignment : public Statement {
public:
	std::string key;
	std::string value;
	char type;

	Assignment(std::string p_key, std::string p_value);

	~Assignment();

	void coutAssignment();

	float execute(std::vector <Assignment>* variables) {
		return -1.0;
	}

	void execute(std::vector <Assignment>* variables, int phase) {
	}
};

class BinaryOperation : public Statement {
public:
	BinaryOperation* right;
	std::string left;
	std::string operation;
	BinaryOperation(std::vector <Token>* t);
	BinaryOperation(std::string p_left, std::string p_operation, BinaryOperation* p_right);
	~BinaryOperation();
	void makeOperation(std::vector<Token>* list_of_tokens, int p_op_needed);
	void coutBinaryOperation();
	float execute(std::vector <std::vector <Assignment>*> variables);
	bool execute(std::vector <std::vector <Assignment>*> vairables, int phase);
};

