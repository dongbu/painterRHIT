#pragma once

#include "paint_util.h"

namespace paint_util{
	std::string string_format(const std::string fmt, ...) {
		int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
		std::string str;
		va_list ap;
		while (1) {     // Maximum two passes on a POSIX system...
			str.resize(size);
			va_start(ap, fmt);
			int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
			va_end(ap);
			if (n > -1 && n < size) { str.resize(n); return str; }
			if (n > -1) size = n + 1; else size *= 2;
		}
		return str;
	}

		// don't put things in this helper file that use non-standard g++ libraries
	/* std::vector<int> getQColor(QString col) {
		std::string col2 = col.toUtf8().constData();
		return getColor(col2);
	} */

	std::vector<int> getColor(std::string col){
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
}