#include <helperMethods.h>

std::vector<int> helperMethods::getColor(std::string col) {
	std::vector<int> toReplace;
	if (col == "black") {
		toReplace.push_back(0);
		toReplace.push_back(0);
		toReplace.push_back(0);
	}
	else if (col == "orange") {
		toReplace.push_back(30);
		toReplace.push_back(144);
		toReplace.push_back(255);
	}
	else if (col == "yellow") {
		toReplace.push_back(0);
		toReplace.push_back(255);
		toReplace.push_back(255);
	}
	else if (col == "green") {
		toReplace.push_back(34);
		toReplace.push_back(139);
		toReplace.push_back(34);
	}
	else if (col == "red") {
		toReplace.push_back(34);
		toReplace.push_back(34);
		toReplace.push_back(178);
	}
	else if (col == "blue") {
		toReplace.push_back(255);
		toReplace.push_back(144);
		toReplace.push_back(30);
	}
	else if (col == "purple") {
		toReplace.push_back(240);
		toReplace.push_back(32);
		toReplace.push_back(160);
	}
	else if (col == "dark grey"){
		toReplace.push_back(75);
		toReplace.push_back(75);
		toReplace.push_back(75);
	}
	else if (col == "medium grey"){
		toReplace.push_back(150);
		toReplace.push_back(150);
		toReplace.push_back(150);
	}
	else if (col == "light grey"){
		toReplace.push_back(225);
		toReplace.push_back(225);
		toReplace.push_back(225);
	}
	else if (col == "white"){
		toReplace.push_back(255);
		toReplace.push_back(255);
		toReplace.push_back(255);
	}
	return toReplace;
}