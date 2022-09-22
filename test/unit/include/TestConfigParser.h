#pragma once

#include "cutest/CuTestCPP.h"

#include "dreg/ConfigParser.h"
#include "dreg/ConfigManager.h"

class TestConfigParser : public BaseCuTest {
public:
	TestConfigParser() {
		testFunctions.reserve(6);
		ADD_TEST(testFunctions, TestConfigParser, invalidConfig);
		ADD_TEST(testFunctions, TestConfigParser, emptyConfig);
		ADD_TEST(testFunctions, TestConfigParser, oneVariableConfig);
		ADD_TEST(testFunctions, TestConfigParser, completeConfig);
		ADD_TEST(testFunctions, TestConfigParser, serializeConfigTest);
		ADD_TEST(testFunctions, TestConfigParser, dregApi);
	}
	
	void testStart() override;
	void testEnd() override;
	void before() override;
	void after() override;

private:
	void invalidConfig();
	void emptyConfig();
	void oneVariableConfig();
	void completeConfig();
	void serializeConfigTest();
	void dregApi();
	
	void checkFloatList(const FloatList* floatList, const float* expectedValues, size_t expectedSize) const;
	
	ConfigParser* configParser;
	
	ConfigManager* configManager;
	ConfigManager* loadedConfigManager;
	VehicleConfig* config;
	VehicleConfig* loadedConfig;
};