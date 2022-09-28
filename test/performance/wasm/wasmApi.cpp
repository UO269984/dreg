#include <emscripten/bind.h>

#include "testPerformance.h"

#define PTR intptr_t

static emscripten::val printFuncObj = emscripten::val("0");
void setTestPrintFuncApi(emscripten::val func) {
	printFuncObj = func;
	setTestPrintFunc([](const char* msg) {
		printFuncObj.call<void>("func", emscripten::val(std::string(msg)));
	});
}

PTR createConfig() {
	return (PTR) new TestsConfig();
}
void deleteConfig(PTR testsConfig) {
	deleteTestsConfig((TestsConfig*) testsConfig);
	delete (TestsConfig*) testsConfig;
}

void updateConfigObj(PTR testsConfigPtr, emscripten::val obj) {
	TestsConfig* testsConfig = (TestsConfig*) testsConfigPtr;
	
	obj.set("bezierSamples", emscripten::val(testsConfig->bezierSamples));
	obj.set("singleVehicleIters", emscripten::val(testsConfig->singleVehicleIters));
	obj.set("multipleVehiclesIters", emscripten::val(testsConfig->multipleVehiclesIters));
	obj.set("multipleVehiclesCount", emscripten::val(testsConfig->multipleVehiclesCount));
	obj.set("createVehiclesCount", emscripten::val(testsConfig->createVehiclesCount));
	obj.set("loadConfigIters", emscripten::val(testsConfig->loadConfigIters));
	
	obj.set("vehicleConfigVehicle", emscripten::val((const char*) testsConfig->vehicleConfigVehicle));
	obj.set("vehicleConfigLoadConfig", emscripten::val((const char*) testsConfig->vehicleConfigLoadConfig));
}

void updateConfigStruct(PTR testsConfigPtr, emscripten::val obj) {
	TestsConfig* testsConfig = (TestsConfig*) testsConfigPtr;
	
	testsConfig->bezierSamples = obj["bezierSamples"].as<size_t>();
	testsConfig->singleVehicleIters = obj["singleVehicleIters"].as<size_t>();
	testsConfig->multipleVehiclesIters = obj["multipleVehiclesIters"].as<size_t>();
	testsConfig->multipleVehiclesCount = obj["multipleVehiclesCount"].as<size_t>();
	testsConfig->createVehiclesCount = obj["createVehiclesCount"].as<size_t>();
	testsConfig->loadConfigIters = obj["loadConfigIters"].as<size_t>();
	
	std::string configVehicle = obj["vehicleConfigVehicle"].as<std::string>();
	std::string configLoadConfig = obj["vehicleConfigLoadConfig"].as<std::string>();
	
	deleteTestsConfig(testsConfig);
	testsConfig->vehicleConfigVehicle = cloneCharArray(configVehicle.c_str(), configVehicle.size() + 1);
	testsConfig->vehicleConfigLoadConfig = cloneCharArray(configLoadConfig.c_str(), configLoadConfig.size() + 1);
}

void loadDefaultConfigApi(PTR testsConfig) {
	loadDefaultConfig((TestsConfig*) testsConfig);
}
bool runTestApi(PTR testsConfig) {
	return allTests((TestsConfig*) testsConfig);
}

EMSCRIPTEN_BINDINGS(drivingEngine) {
	emscripten::function("setTestPrintFunc", &setTestPrintFuncApi, emscripten::allow_raw_pointers());
	
	emscripten::function("createConfig", &createConfig, emscripten::allow_raw_pointers());
	emscripten::function("deleteConfig", &deleteConfig, emscripten::allow_raw_pointers());
	emscripten::function("updateConfigObj", &updateConfigObj, emscripten::allow_raw_pointers());
	emscripten::function("updateConfigStruct", &updateConfigStruct, emscripten::allow_raw_pointers());
	
	emscripten::function("loadDefaultConfig", &loadDefaultConfigApi, emscripten::allow_raw_pointers());
	emscripten::function("runTest", &runTestApi, emscripten::allow_raw_pointers());
}