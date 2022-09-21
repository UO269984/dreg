#pragma once

#include "cutest/CuTestCPP.h"
#include "dreg/Graph.h"

class TestGraph : public BaseCuTest {
public:
	TestGraph() {
		testFunctions.reserve(6);
		ADD_TEST(testFunctions, TestGraph, testLinear);
		ADD_TEST(testFunctions, TestGraph, testBezier);
		ADD_TEST(testFunctions, TestGraph, invalidGraph);
		ADD_TEST(testFunctions, TestGraph, initData);
		ADD_TEST(testFunctions, TestGraph, clone);
		ADD_TEST(testFunctions, TestGraph, dregApi);
	}
	
	void testStart() override;
	void testEnd() override;
	void before() override;
	void after() override;

private:
	void testLinear();
	void testBezier();
	void invalidGraph();
	void initData();
	void clone();
	void dregApi();
	
	void checkGraphValues(const Graph* graph, const Vector2* values, size_t valuesCount) const;
	
	Graph* graph;
	Graph* dregApiGraph = NULL;
};