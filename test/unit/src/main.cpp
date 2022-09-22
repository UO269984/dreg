#include <stdio.h>

#include "cutest/CuTestCPP.h"
#include "dreg/dreg.h"

#include "TestVector3.h"
#include "TestFloatList.h"
#include "TestGraph.h"
#include "TestInputLogger.h"

#include <cstring>

int main(int argc, const char** argv) {
	if (argc > 1 && strcmp(argv[1], "--no-dreg-print") == 0)
		setPrintFunc([](const char* msg) {});
	
	CuSuiteWrapper suite;
	TestVector3 testVector3;
	TestFloatList testFloatList;
	TestGraph testGraph;
	TestInputLogger testInputLogger;
	
	suite.addTest(&testVector3);
	suite.addTest(&testFloatList);
	suite.addTest(&testGraph);
	suite.addTest(&testInputLogger);
	suite.run();
	suite.printResult();
	
	return 0;
}