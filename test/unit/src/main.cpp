#include <stdio.h>

#include "cutest/CuTestCPP.h"
#include "dreg/dreg.h"

#include <cstring>

int main(int argc, const char** argv) {
	if (argc > 1 && strcmp(argv[1], "--no-dreg-print") == 0)
		setPrintFunc([](const char* msg) {});
	
	CuSuiteWrapper suite;
	
	suite.run();
	suite.printResult();
	
	return 0;
}