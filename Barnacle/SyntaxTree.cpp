#include "iostream"
#include "Syntaxtree.h"
#include "Block.h"
#include "Lexer.h"

std::vector <std::vector <std::string>> declared_vars;

SyntaxTree::SyntaxTree() {
	executedFunction = nullptr;
}

SyntaxTree::SyntaxTree(Lexer lexed_info) {
	executedFunction = nullptr;
}

SyntaxTree::~SyntaxTree() {

}

void SyntaxTree::getBlocks(Lexer* lexed_info) {
	std::cout << lexed_info->size();
	do {
		this->Blocks.push_back(lexed_info->feed());
		if (this->Blocks.at(this->Blocks.size() - 1).isEmpty()) {
			this->Blocks.pop_back();
		}
	} while (!lexed_info->isEmpty());
}

void SyntaxTree::coutTree() {
	for (int i = 0; i < this->Blocks.size(); i++) {
		std::cout << "Block " << i << ":\n";
		Blocks.at(i).coutBlock();
		std::cout << "End of Block... \n\n";
	}
}