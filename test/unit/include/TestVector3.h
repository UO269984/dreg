#pragma once

#include "cutest/CuTestCPP.h"
#include "dreg/Vector3.h"

class TestVector3 : public BaseCuTest {
public:
	TestVector3() {
		testFunctions.reserve(6);
		ADD_TEST(testFunctions, TestVector3, compare);
		ADD_TEST(testFunctions, TestVector3, sum);
		ADD_TEST(testFunctions, TestVector3, subtraction);
		ADD_TEST(testFunctions, TestVector3, mult);
		ADD_TEST(testFunctions, TestVector3, increment);
		ADD_TEST(testFunctions, TestVector3, length);
	}

private:
	void compare();
	void sum();
	void subtraction();
	void mult();
	void increment();
	void length();
	
	const Vector3 zero1 = {0, 0, 0};
	const Vector3 zero2 = {0, 0, 0};
	const Vector3 zeroOne1 = {0, 0, 1};
	const Vector3 zeroOne2 = {0, 0, 1};
	const Vector3 v1a = {1, 2, 3};
	const Vector3 v1b = {1, 2, 3};
	const Vector3 v2 = {3, 2, 3};
};