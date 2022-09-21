#pragma once

#include "cutest/CuTestCPP.h"
#include "dreg/VehicleApi.h"

class TestFloatList : public BaseCuTest {
public:
	TestFloatList() {
		ADD_TEST(testFunctions, TestFloatList, test);
	}
	
	void after() override;

private:
	void test();
	
	FloatList* floatList = NULL;
};