#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Token.h"

bool stringEquals(std::string str_1, std::string str_2);

int isMember(std::string element, int size, std::vector <std::vector <std::string>> vector_of_elements);

// Determines if a given elment is a member of a given vector of elements. This requires the vector
//    to be one dimensional. True is returned if the element is a member of the given vector and 
//    false is returned otherwise.
template <class member_type>
bool isMember(member_type element, std::vector <member_type> vector_of_elements) {
	bool res = false;
	for (int i = 0; i < vector_of_elements.size(); i++) {
		if (element == vector_of_elements.at(i)) {
			res = true;
			break;
		}
	}
	return res;
}

bool isNaN(std::string str);

void getTag(std::string key, std::vector <std::vector <std::string>> array_of_elements,
	std::string base_value, std::vector <Token>* edit_array);

std::vector <std::vector <std::string>> vectorAppend(std::vector <std::vector <std::string>> vec_1,
	std::vector <std::vector <std::string>> vec_2);

int toNum(std::string value);

//pushFront places a given value at the front of a given vector
template <class vec_type>
std::vector<vec_type> pushFront(vec_type value, std::vector <vec_type>* vec) {
	std::vector<vec_type> tempVec = { value };
	const int size = vec->size();
	for (int i = 0; i < size; i++) {
		tempVec.push_back(vec->at(i));
	}
	return tempVec;
}