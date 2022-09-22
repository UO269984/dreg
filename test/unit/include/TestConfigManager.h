#pragma once

#include "cutest/CuTestCPP.h"
#include "dreg/ConfigManager.h"

class TestConfigManager : public BaseCuTest {
public:
	TestConfigManager() {
		testFunctions.reserve(4);
		ADD_TEST(testFunctions, TestConfigManager, constructor);
		ADD_TEST(testFunctions, TestConfigManager, vehicles);
		ADD_TEST(testFunctions, TestConfigManager, clone);
		ADD_TEST(testFunctions, TestConfigManager, dregApi);
	}
	
	void testStart() override;
	void testEnd() override;
	void before() override;
	void after() override;

private:
	void constructor();
	void vehicles();
	void clone();
	void dregApi();
	
	void checkGraph(const Graph* graph, bool expectedObjects) const;
	void checkPointersLoaded(const VehicleConfig* config, bool expectedGraph) const;
	
	ConfigManager* configManager;
	VehicleConfig* config;
};