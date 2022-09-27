#pragma once

#include <cstddef>
#include "dreg/dreg.h"

struct TestsConfig {
	size_t bezierSamples;
	size_t singleVehicleIters;
	size_t multipleVehiclesIters;
	size_t multipleVehiclesCount;
	size_t createVehiclesCount;
	size_t loadConfigIters;
	
	char* vehicleConfigVehicle = NULL;
	char* vehicleConfigLoadConfig = NULL;
};

void setTestPrintFunc(PrintFunc newPrintFunc);
bool allTests(const TestsConfig* config);

char* cloneCharArray(const char* str, size_t size);
void deleteTestsConfig(TestsConfig* config);
void loadDefaultConfig(TestsConfig* config);