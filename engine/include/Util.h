#pragma once

#include <list>
#include <string_view>

namespace Util {
	void split(const std::string_view str, char splitChar, std::list<std::string_view>& splitted);
	std::string_view removeBlankEnds(const std::string_view str);
	bool strToFloat(const std::string_view str, float* converted);
}