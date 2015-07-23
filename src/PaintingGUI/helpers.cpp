#pragma once
// any generic helper functions

#include <string>
#include <cstdarg> //gunnar needs this to compile (AC: why?... put reason here for potential future deletion)
//GH: The library cstdarg allows my computer to recognize va_start && va_end

class Helpers  {
public:
	static std::string string_format(const std::string fmt, ...) {
		int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
		std::string str;
		va_list ap;
		while (1) {     // Maximum two passes on a POSIX system...
			str.resize(size);
			va_start(ap, fmt);
			int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
			va_end(ap);
			if (n > -1 && n < size) {  // Everything worked
				str.resize(n);
				return str;
			}
			if (n > -1)  // Needed size returned
				size = n + 1;   // For null char
			else
				size *= 2;      // Guess at a larger size (OS specific)
		}
		return str;
	}
};
