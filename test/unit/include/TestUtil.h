#pragma once

#include "cutest/CuTestCPP.h"
#include "dreg/VehicleApi.h"
#include "dreg/Graph.h"

#include <cstring>

namespace TestUtil {
	
	void checkFloatList(CuTest* tc, const FloatList* floatList, const float* expectedValues, size_t expectedSize);
	void checkGraphPoints(CuTest* tc, const Graph* graph, const Vector2* expectedPoints, size_t expectedPointsCount);
	void checkInitdata(CuTest* tc, const Graph* graph,
		GraphType expectedType, const Vector2* expectedRefs, size_t expectedRefsCount);
}