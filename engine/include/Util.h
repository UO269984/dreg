#pragma once

#include <list>
#include <string_view>

namespace Util {
	void split(const std::string_view str, char splitChar, std::list<std::string_view>& splitted);
	std::string_view removeBlankEnds(const std::string_view str);
	
	bool strToInt(const std::string_view str, int* converted);
	bool strToFloat(const std::string_view str, float* converted);
}