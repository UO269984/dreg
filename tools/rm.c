#include <stdio.h>

int main(int argc, char const* argv[]) {
	for (int i = 1; i < argc; i++) {
		const char* curParam = argv[i];
		
		if (curParam[0] != '-')
			remove(curParam);
	}
	return 0;
}