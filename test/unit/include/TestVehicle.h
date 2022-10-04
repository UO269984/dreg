#pragma once

#include "cutest/CuTestCPP.h"
#include "dreg/VehicleApi.h"

class TestVehicle : public BaseCuTest {
public:
	TestVehicle() {
		ADD_TEST(testFunctions, TestVehicle, dregApi);
	}
	
	void testStart() override;
	void testEnd() override;

private:
	void dregApi();
	
	ConfigManager* configManager;
};