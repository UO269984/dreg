#pragma once

#include "cutest/CuTestCPP.h"
#include "dreg/InputLogger.h"

class TestInputLogger : public BaseCuTest {
public:
	TestInputLogger() {
		testFunctions.reserve(4);
		ADD_TEST(testFunctions, TestInputLogger, noRecords);
		ADD_TEST(testFunctions, TestInputLogger, oneRecord);
		ADD_TEST(testFunctions, TestInputLogger, multipleRecords);
		ADD_TEST(testFunctions, TestInputLogger, dregApi);
	}
	
	void testStart() override;
	void testEnd() override;
	void before() override;
	void after() override;

private:
	void noRecords();
	void oneRecord();
	void multipleRecords();
	void dregApi();
	
	Vehicle* vehicle;
	InputLogger* inputLogger;
	
	static void recordFileSave(const char* filename, const char* data);
	static std::string lastFileSave;
};