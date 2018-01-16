#include <string>
#include <vector>
#include "Token.h"
#include "ToolFunctions.h"

// Determines if two strings are equal.
bool stringEquals(std::string str_1, std::string str_2) {
	if (str_1.empty() && str_2.empty()) {
		return true;
	}
	else if (str_1.empty() || str_2.empty()) {
		return false;
	}
	else if (str_1[0] == str_2[0]) {
		return stringEquals(str_1.substr(1), str_2.substr(1));
	}
	else {
		return false;
	}
}

// This fuction is for nested vectors (2-dimensional)
// Determines if an element in a given vector of vectors of strings (the first element in the nested vector)
//    can be found in the last letters of a given word. If so, the index of the string where the element
//    can be found is returned. If no element can be found, -1 is returned.
int isMember(std::string element, int size, std::vector <std::vector <std::string>> vector_of_elements) {
	int res = -1;
	for (int i = 0; i < size; i++) {
		if (stringEquals(element, vector_of_elements[i][0])) {
			res = 0;
			break;
		}
		else if (vector_of_elements[i][0].length() < element.length()) {
			if (stringEquals(element.substr(element.length() - vector_of_elements[i][0].length()),
				vector_of_elements[i][0])) {
				res = element.length() - vector_of_elements[i][0].length();
				break;
			}
		}
	}
	return res;
}

// takes in a string and determines if every character in the string is a number
bool isNaN(std::string str) {
	bool res = false;
	for (int i = 0; i < str.length(); i++) {
		if (!(((int)str[i] >= 48 && (int)str[i] <= 57) || str[i] == '.')) {
			res = true;
		}
	}
	return res;
}

// Adds the appropriate token to a vector of tokens given a certain key, a vector
//    of keywords (so that the right tag can be found) and a default tag if no
//    such keyword in the vector matches the given key.
void getTag(std::string key, std::vector <std::vector <std::string>> array_of_elements,
	std::string base_value, std::vector <Token>* edit_array) {
	bool found = false;
	for (int index = 0; index < array_of_elements.size(); index++) {
		if (key == array_of_elements[index][0]) {
			edit_array->push_back(Token(key, array_of_elements[index][1]));
			found = true;
		}
	}
	if (!found && !isNaN(key)) {
		edit_array->push_back(Token(key, "Integer"));
	}
	else if (!found) {
		edit_array->push_back(Token(key, base_value));
	}
}

// vectorAppend appends two vectors of vectors of strings to each other.
std::vector <std::vector <std::string>> vectorAppend(std::vector <std::vector <std::string>> vec_1,
	std::vector <std::vector <std::string>> vec_2) {
	for (int index = 0; index < vec_2.size(); index++) {
		vec_1.push_back(vec_2[index]);
	}
	return vec_1;
}

// Converts a string of digits in to the cooresponding integer.
int toNum(std::string value) {
	int answer = 0;
	int sign = 1;
	if (value == "true") {
		return 1;
	}else if(value == "false"){
		return 0;
	}else if (value.length() < 1) {
		return 0;
	}
	else if (value.length() == 1) {
	}
	else if (value[0] == '-' && value[1] != '-') {
		sign = -1;
		value = value.substr(1);
	}
	for (int i = 0; value.length() > i && ((((int)value[i]) - 48) < 10 && (((int)value[i]) - 48) >= 0); i++) {
		answer = (10 * answer) + (((int)value[i]) - 48);
	}

	return answer*sign;
}