#pragma once

#include "cutest/CuTestCPP.h"
#include "dreg/VehicleApi.h"

class TestFloatList : public BaseCuTest {
public:
	TestFloatList() {
		ADD_TEST(testFunctions, TestFloatList, test);
	}

private:
	void test();
};