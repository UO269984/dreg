#include "Util.h"

#include <stdlib.h>
#include <inttypes.h>

namespace Util {
	void split(const std::string_view str, char splitChar, std::list<std::string_view>& splitted) {
		size_t blockEndPos = 0;
		size_t blockStartPos = 0;
		
		while ((blockEndPos = str.find(splitChar, blockStartPos)) != str.npos) {
			int lineLength = blockEndPos - blockStartPos;
			
			if (lineLength > 0)
				splitted.push_back(str.substr(blockStartPos, lineLength));
			
			blockStartPos = blockEndPos + 1;
		}
		if (blockStartPos != str.size())
			splitted.push_back(str.substr(blockStartPos));
	}

	std::string_view removeBlankEnds(const std::string_view str) {
		size_t startPos = str.find_first_not_of(" \t");
		return startPos == str.npos ? std::string_view() :
			str.substr(startPos, str.find_last_not_of(" \t") - startPos + 1);
	}
	
	bool strToInt(const std::string_view str, int* converted) {
		char aux[30];
		str.copy(aux, 29);
		aux[std::min(str.size(), (size_t) 29)] = 0;
		
		char* endptr;
		*converted = strtoimax(aux, &endptr, 10);
		return endptr == aux + str.size();
	}
	
	bool strToFloat(const std::string_view str, float* converted) {
		char aux[30];
		str.copy(aux, 29);
		aux[std::min(str.size(), (size_t) 29)] = 0;
		
		char* endptr;
		*converted = strtof(aux, &endptr);
		return endptr == aux + str.size();
	}
}