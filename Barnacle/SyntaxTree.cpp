#include "iostream"
#include "Syntaxtree.h"
#include "Block.h"
#include "Lexer.h"

std::vector <std::vector <std::string>> declared_vars;

SyntaxTree::SyntaxTree() {

}

SyntaxTree::SyntaxTree(Lexer lexed_info) {

}

SyntaxTree::~SyntaxTree() {

}

void SyntaxTree::getBlocks(Lexer* lexed_info) {
	do {
		this->Blocks.push_back(lexed_info->feed());
	} while (!this->Blocks.at(this->Blocks.size() - 1).isEmpty());
	this->Blocks.pop_back();
}

void SyntaxTree::getStatements(std::vector <Assignment>* variables, std::vector <Function>* functions) {
	for (int i = 0; i < this->Blocks.size(); i++) {
		this->Blocks.at(i).evaluate({ variables }, functions);
	}
}

void SyntaxTree::coutTree() {
	for (int i = 0; i < this->Blocks.size(); i++) {
		std::cout << "Block " << i << ":\n";
		Blocks.at(i).coutBlock();
		std::cout << "End of Block... \n\n";
	}
}