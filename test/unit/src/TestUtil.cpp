#include "TestUtil.h"

#include "dreg/dreg.h"

void TestUtil::checkFloatList(CuTest* tc, const FloatList* floatList, const float* expectedValues, size_t expectedSize) {
	size_t size;
	const float* values = getFloatList(floatList, &size);
	
	CuAssertIntEquals(expectedSize, size);
	CuAssertTrue(memcmp(expectedValues, values, sizeof(float) * size) == 0);
}

void TestUtil::checkGraphPoints(CuTest* tc, const Graph* graph, const Vector2* expectedPoints, size_t expectedPointsCount) {
	size_t pointsCount;
	const Vector2* points = graph->getPoints(&pointsCount);
	
	CuAssertIntEquals(expectedPointsCount, pointsCount);
	CuAssertTrue(memcmp(expectedPoints, points, sizeof(Vector2) * pointsCount) == 0);
}

void TestUtil::checkInitdata(CuTest* tc, const Graph* graph,
		GraphType expectedType, const Vector2* expectedRefs, size_t expectedRefsCount) {
	
	const GraphInitData* initData = graph->getInitData();
	
	CuAssertPtrNotNull(initData);
	CuAssertTrue(initData->type == expectedType);
	CuAssertIntEquals(expectedRefsCount, initData->refs.size());
	CuAssertTrue(memcmp(expectedRefs, initData->refs.data(), sizeof(Vector2) * expectedRefsCount) == 0);
}