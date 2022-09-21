#pragma once

#define CPP_SUPPORT
#include "CuTest.h"

#include <vector>
#include <list>
#include <string>

#define GET_TEST_FUNC(CLASS, METHOD) std::bind(&CLASS::METHOD, this)
#define ADD_TEST(TESTS_VECTOR, CLASS, METHOD) TESTS_VECTOR.emplace_back(GET_TEST_FUNC(CLASS, METHOD), #CLASS "." #METHOD)

struct TestFuncData {
	TestFuncData(const std::function<void()>& func, const char* name) : func(func), name(name) {}
	std::function<void()> func;
	const char* name;
};

class BaseCuTest {
public:
	virtual void testStart() {};
	virtual void testEnd() {};
	virtual void before() {};
	virtual void after() {};
	
	const std::vector<TestFuncData>* getTestFuncs() const;
	CuTest* tc;

protected:
	std::vector<TestFuncData> testFunctions;
};

struct CuTestResult {
	bool success;
	const char* summary;
	const char* details;
};

class CuSuiteWrapper {
public:
	CuSuiteWrapper();
	~CuSuiteWrapper();
	
	void addTest(BaseCuTest* test);
	bool run();
	CuTestResult getTestResult() const;
	void printResult() const;

private:
	void runTest(CuTest* tc);
	void nextTest();
	
	CuSuite* suite;
	std::list<BaseCuTest*> baseTests;
	std::list<BaseCuTest*>::iterator testsIt;
	
	unsigned int testIndex;
	
	bool testSuccess = false;
	std::string summary;
	std::string details;
};