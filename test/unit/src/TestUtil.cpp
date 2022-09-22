#include "TestUtil.h"

#include "dreg/dreg.h"

void TestUtil::checkEqualFloatLists(CuTest* tc, const FloatList* list1, const FloatList* list2) {
	checkFloatList(tc, list1, list2->data(), list2->size());
}

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

void TestUtil::checkSameGraphs(CuTest* tc, const Graph* graph1, const Graph* graph2) {
	size_t pointsCount2;
	const Vector2* points2 = graph2->getPoints(&pointsCount2);
	checkGraphPoints(tc, graph1, points2, pointsCount2);
}

void TestUtil::checkSameConfigs(CuTest* tc, const VehicleConfig* config1, const VehicleConfig* config2, bool fullCopy) {
	if (fullCopy) {
		CuAssertTrue(config1->frontShaft == config2->frontShaft);
		CuAssertTrue(config1->rearShaft == config2->rearShaft);
		CuAssertDblEquals(config1->maxSteeringAngle, config2->maxSteeringAngle, 0.00001);
		CuAssertDblEquals(config1->mass, config2->mass, 0.00001);
		
		CuAssertDblEquals(config1->power.torqueToRpmAccel, config2->power.torqueToRpmAccel, 0.00001);
		CuAssertDblEquals(config1->power.driveRatio, config2->power.driveRatio, 0.00001);
		
		CuAssertDblEquals(config1->wheels.diameter, config2->wheels.diameter, 0.00001);
		CuAssertDblEquals(config1->wheels.brakeDiameter, config2->wheels.brakeDiameter, 0.00001);
		CuAssertDblEquals(config1->wheels.brakeStaticFrictionCoef, config2->wheels.brakeStaticFrictionCoef, 0.00001);
		CuAssertDblEquals(config1->wheels.brakeKineticFrictionCoef, config2->wheels.brakeKineticFrictionCoef, 0.00001);
		
		checkSameGraphs(tc, config1->power.throttleCurve, config2->power.throttleCurve);
		checkSameGraphs(tc, config1->power.engineCurve, config2->power.engineCurve);
		checkSameGraphs(tc, config1->power.looseEngineRpmCurve, config2->power.looseEngineRpmCurve);
		checkSameGraphs(tc, config1->power.engineBrakeCurve, config2->power.engineBrakeCurve);
		checkSameGraphs(tc, config1->power.clutchCurve, config2->power.clutchCurve);
		checkSameGraphs(tc, config1->brakeCurve, config2->brakeCurve);
		checkEqualFloatLists(tc, config1->power.gearRatios, config2->power.gearRatios);
	}
	else
		CuAssertTrue(memcmp(config1, config2, sizeof(VehicleConfig)) == 0);
}