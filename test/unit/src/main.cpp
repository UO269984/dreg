#include <stdio.h>

#include "cutest/CuTestCPP.h"
#include "dreg/dreg.h"

#include "DregLogManager.h"
#include "TestVector3.h"
#include "TestFloatList.h"
#include "TestGraph.h"
#include "TestConfigManager.h"
#include "TestConfigParser.h"
#include "TestVehicle.h"
#include "TestInputLogger.h"

#include <cstring>

int main(int argc, const char** argv) {
	dregLogManager.printEnabled = argc <= 1 || strcmp(argv[1], "--no-dreg-print") != 0;
	setPrintFunc([](const char* msg) {dregLogManager.newLog(msg);});
	
	CuSuiteWrapper suite;
	TestVector3 testVector3;
	TestFloatList testFloatList;
	TestGraph testGraph;
	TestConfigManager testConfigManager;
	TestConfigParser testConfigParser;
	TestVehicle testVehicle;
	TestInputLogger testInputLogger;
	
	suite.addTest(&testVector3);
	suite.addTest(&testFloatList);
	suite.addTest(&testGraph);
	suite.addTest(&testConfigManager);
	suite.addTest(&testConfigParser);
	suite.addTest(&testVehicle);
	suite.addTest(&testInputLogger);
	suite.run();
	suite.printResult();
	
	dregLogManager.freeMemory();
	ConfigParser::clearConfigProps();
	return 0;
}