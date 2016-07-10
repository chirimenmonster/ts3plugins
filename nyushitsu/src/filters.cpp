#include <iostream>
#include <regex>

#include "filters.h"

int filter_number(const char *string, char *result, size_t size) {
	std::string src(string);
	std::regex regex(R"(\d+)");
	
	std::string replacedString = std::regex_replace(src, regex, " ");
	strncpy(result, replacedString.c_str(), size);

	return 0;
}
