#include "TestGraph.h"
#include "TestUtil.h"

#include "dreg/dreg.h"

void TestGraph::testStart() {
	graph = (Graph*) malloc(sizeof(Graph));
}

void TestGraph::testEnd() {
	setGraphSaveInitData(0);
	free(graph);
	
	if (dregApiGraph != NULL)
		deleteGraph(dregApiGraph);
}

void TestGraph::before() {
	setGraphSaveInitData(0);
	new(graph) Graph();
}

void TestGraph::after() {
	graph->~Graph();
}

void TestGraph::testLinear() {
	Vector2 refs[] = {{0, 0}, {1, 1}, {1.5, 0}};
	CuAssertTrue(graph->loadLinear(refs, 2)); //2 References: (0, 0), (1, 1)
	
	Vector2 values1[] = {{0, 0}, {1, 1}, {-0.1, 0}, {1.1, 1},
		{-8000, 0}, {8000, 1}, {0.5, 0.5}, {0.3, 0.3}, {0.95, 0.95}};
	TestUtil::checkGraphPoints(tc, graph, refs, 2);
	checkGraphValues(graph, values1, 9);
	
	refs[1] = {0.5, 1};
	Vector2 values2[] = {{0, 0}, {1, 0.5}, {-0.1, 0}, {1.1, 0.4},
		{-8000, 0}, {8000, 0}, {0.5, 1}, {0.3, 0.6}, {0.95, 0.55}};
	CuAssertTrue(graph->loadLinear(refs, 3)); //3 References: (0, 0), (0.5, 1), (1.5, 0)
	TestUtil::checkGraphPoints(tc, graph, refs, 3);
	checkGraphValues(graph, values2, 9);
	
	refs[0] = {0.3, 0.3};
	Vector2 values3[] = {{0.3, 0.3}, {0, 0.3}, {1, 0.3},
		{-5000, 0.3}, {5000, 0.3}, {0.125, 0.3}, {0.72, 0.3}}; //Horizontal graph
	CuAssertTrue(graph->loadLinear(refs, 1)); //1 References: (0.3, 0.3)
	TestUtil::checkGraphPoints(tc, graph, refs, 1);
	checkGraphValues(graph, values3, 7);
}

void TestGraph::testBezier() {
	Vector2 refs[] = {{0, 0}, {0.2, 0.2}, {0.8, 0.8}, {1, 1}, {0.6, 0.8}, {0.8, 0.8}, {1, 1}};
	CuAssertTrue(graph->loadBezier(refs, 4)); //Bezier curve aligned as linear curve (0, 0), (1, 1)
	
	Vector2 values1[] = {{0, 0}, {1, 1}, {-0.1, 0}, {1.1, 1},
		{-8000, 0}, {8000, 1}, {0.5, 0.5}, {0.3, 0.3}, {0.95, 0.95}};
	checkGraphValues(graph, values1, 9);
	
	refs[2] = {0.2, 0.4};
	refs[3] = {0.4, 0.6};
	CuAssertTrue(graph->loadBezier(refs, 7)); //Bezier curve through (0, 0), (0.4, 0.6), (1, 1)
	Vector2 values2[] = {{0, 0}, {1, 1}, {-0.1, 0}, {1.1, 1},
		{-8000, 0}, {8000, 1}, {0.4, 0.6}, {0.2, 0.3}, {0.7, 0.8}};
	checkGraphValues(graph, values2, 9);
}

void TestGraph::checkGraphValues(const Graph* graph, const Vector2* values, size_t valuesCount) const {
	for (size_t i = 0; i < valuesCount; i++)
		CuAssertDblEquals(values[i].y, graph->getY(values[i].x), 0.000001);
}

void TestGraph::invalidGraph() {
	Vector2 refs[] = {{0, 0}, {1, 1}};
	CuAssertTrue(! graph->loadLinear(refs, 0));
	
	CuAssertTrue(! graph->loadBezier(refs, 0));
	CuAssertTrue(! graph->loadBezier(refs, 1));
	CuAssertTrue(! graph->loadBezier(refs, 3));
	CuAssertTrue(! graph->loadBezier(refs, 5));
	CuAssertTrue(! graph->loadBezier(refs, 6));
}

void TestGraph::initData() {
	//Save init data disabled
	CuAssertTrue(graph->getInitData() == NULL);
	
	Vector2 refs[] = {{0, 0}, {0.2, 0.2}, {0.8, 0.8}, {1, 1}};
	CuAssertTrue(graph->loadLinear(refs, 2));
	CuAssertTrue(graph->getInitData() == NULL);
	
	CuAssertTrue(graph->loadBezier(refs, 4));
	CuAssertTrue(graph->getInitData() == NULL);
	
	//Save init data enabled
	setGraphSaveInitData(1);
	graph->~Graph(); //Recreate graph
	new(graph) Graph(); //New value of saveInitData only applies to new graphs
	
	CuAssertTrue(graph->loadLinear(refs, 2));
	TestUtil::checkInitdata(tc, graph, GraphType::LINEAR, refs, 2);
	
	CuAssertTrue(graph->loadBezier(refs, 4));
	TestUtil::checkInitdata(tc, graph, GraphType::BEZIER, refs, 4);
}

void TestGraph::clone() {
	Graph cloned(*graph);
	TestUtil::checkGraphPoints(tc, &cloned, NULL, 0);
	
	Vector2 refs[] = {{0.1, 0.4}, {0.2, 0.2}, {10, 50}};
	CuAssertTrue(graph->loadLinear(refs, 3));
	TestUtil::checkGraphPoints(tc, graph, refs, 3);
	TestUtil::checkGraphPoints(tc, &cloned, NULL, 0); //Loading the original graph does not change the copy
	
	cloned.~Graph();
	new(&cloned) Graph(*graph);
	TestUtil::checkGraphPoints(tc, &cloned, refs, 3);
	CuAssertTrue(cloned.getInitData() == NULL);
	
	setGraphSaveInitData(1);
	graph->~Graph(); //Recreate original graph
	new(graph) Graph();
	CuAssertTrue(graph->loadLinear(refs, 3));
	
	CuAssertTrue(cloned.getInitData() == NULL);
	
	cloned.~Graph();
	new(&cloned) Graph(*graph);
	CuAssertPtrNotNull(cloned.getInitData()); //Init data has been copied
	
	TestUtil::checkGraphPoints(tc, &cloned, refs, 3);
	TestUtil::checkInitdata(tc, graph, GraphType::LINEAR, refs, 3);
	TestUtil::checkInitdata(tc, &cloned, GraphType::LINEAR, refs, 3);
	
	CuAssertTrue(cloned.loadLinear(refs, 1)); //Loading graph in the copy does not change the original
	TestUtil::checkInitdata(tc, graph, GraphType::LINEAR, refs, 3);
	TestUtil::checkInitdata(tc, &cloned, GraphType::LINEAR, refs, 1);
}

void TestGraph::dregApi() {
	Vector2 refs[] = {{0.5, 1}, {1, 2}, {3, 6}, {4, 4}};
	dregApiGraph = createGraph();
	loadLinearGraph(dregApiGraph, refs, 4);
	
	//loadLinear / getGraphPoints, getGraphY
	size_t pointsCount;
	const Vector2* points = getGraphPoints(dregApiGraph, &pointsCount);
	CuAssertIntEquals(4, pointsCount);
	CuAssertTrue(memcmp(refs, points, sizeof(Vector2) * pointsCount) == 0);
	CuAssertDblEquals(2, getGraphY(dregApiGraph, 1), 0.00001);
	CuAssertTrue(getGraphInitData(dregApiGraph, NULL, NULL) == NULL);
	
	//loadBezier / getGraphInitData
	setGraphSaveInitData(1);
	deleteGraph(dregApiGraph);
	dregApiGraph = createGraph();
	loadBezierGraph(dregApiGraph, refs, 4, 0);
	
	char isBezier;
	size_t refsCount;
	const Vector2* graphRefs = getGraphInitData(dregApiGraph, &isBezier, &refsCount);
	CuAssertPtrNotNull(graphRefs);
	CuAssertTrue(isBezier);
	CuAssertIntEquals(4, refsCount);
	CuAssertTrue(memcmp(refs, graphRefs, sizeof(Vector2) * refsCount) == 0);
	
	//cloneGraph
	CuAssertTrue(graph->loadLinear(refs, 4));
	deleteGraph(dregApiGraph);
	dregApiGraph = cloneGraph(graph);
	TestUtil::checkGraphPoints(tc, dregApiGraph, refs, 4);
}