#include "dregApi.h"
#include "updateObj.h"

dreg::Vector2 createVector2(float x, float y) {
	return {x, y};
}

dreg::Vector3 createVector3(float x, float y, float z) {
	return {x, y, z};
}

dreg::VehicleControls createVehicleControls(float throttle, float brake, float steeringWheel, float clutch, int gear) {
	return {throttle, brake, steeringWheel, clutch, gear};
}


PTR createVehicle() {return (PTR) dreg::createVehicle();}
void deleteVehicle(PTR vehicle) {dreg::deleteVehicle((dreg::Vehicle*) vehicle);}
void resetVehicle(PTR vehicle) {dreg::resetVehicle((dreg::Vehicle*) vehicle);}

void setVehicleInput(PTR vehicle, dreg::VehicleControls* vehicleControls) {
	dreg::setVehicleInput((dreg::Vehicle*) vehicle, vehicleControls);
}

PTR getVehicleState(PTR vehicle) {
	return (PTR) dreg::getVehicleState((dreg::Vehicle*) vehicle);
}

PTR getVehicleProps(PTR vehicle) {
	return (PTR) dreg::getVehicleProps((dreg::Vehicle*) vehicle);
}

PTR getConfigManager(PTR vehicle) {
	return (PTR) dreg::getConfigManager((dreg::Vehicle*) vehicle);
}

void setVehicleConfig(PTR vehicle, PTR configManager) {
	dreg::setVehicleConfig((dreg::Vehicle*) vehicle, (dreg::ConfigManager*) configManager);
}

void updateVehicle(PTR vehicle, float delta) {
	dreg::update((dreg::Vehicle*) vehicle, delta);
}


PTR createConfigManager(bool createObjects) {
	return (PTR) dreg::createConfigManager(createObjects);
}

void deleteConfigManager(PTR configManager) {
	dreg::deleteConfigManager((dreg::ConfigManager*) configManager);
}

PTR cloneConfig(PTR configManager, bool fullClone) {
	return (PTR) dreg::cloneConfig((dreg::ConfigManager*) configManager, fullClone);
}

PTR getVehicleConfig(PTR configManager) {
	return (PTR) dreg::getVehicleConfig((dreg::ConfigManager*) configManager);
}

void updateConfig(PTR configManager) {
	dreg::updateConfig((dreg::ConfigManager*) configManager);
}

void loadDefaultConfig(PTR configManager) {
	dreg::loadDefaultConfig((dreg::ConfigManager*) configManager);
}

bool loadSerializedConfig(PTR vehicleConfig, emscripten::val serializedConfig) {
	return dreg::loadSerializedConfig((dreg::VehicleConfig*) vehicleConfig, serializedConfig.as<std::string>().c_str());
}

emscripten::val serializeConfig(PTR vehicleConfig) {
	char* serializedPtr = dreg::serializeConfig((dreg::VehicleConfig*) vehicleConfig);
	emscripten::val serializedStr = emscripten::val((const char*) serializedPtr);
	dreg::deleteCharArray(serializedPtr);
	
	return serializedStr;
}


PTR createGraph() {return (PTR) dreg::createGraph();}
void deleteGraph(PTR graph) {dreg::deleteGraph((dreg::Graph*) graph);}
PTR cloneGraph(PTR graph) {return (PTR) dreg::cloneGraph((dreg::Graph*) graph);}

static dreg::Vector2* refsToVector2Array(emscripten::val refs) {
	size_t length = refs["length"].as<size_t>();
	dreg::Vector2* vector2Array = new dreg::Vector2[length];
	
	for (size_t i = 0; i < length; i++)
		vector2Array[i] = {refs[i][0].as<float>(), refs[i][1].as<float>()};
	
	return vector2Array;
}

void loadLinearGraph(PTR graph, emscripten::val refs) {
	dreg::Vector2* refsArray = refsToVector2Array(refs);
	dreg::loadLinearGraph((dreg::Graph*) graph, refsArray, refs["length"].as<size_t>());
	delete[] refsArray;
}

void loadBezierGraph(PTR graph, emscripten::val refs, size_t samplesPerSegment) {
	dreg::Vector2* refsArray = refsToVector2Array(refs);
	dreg::loadBezierGraph((dreg::Graph*) graph, refsArray, refs["length"].as<size_t>(), samplesPerSegment);
	delete[] refsArray;
}

emscripten::val getGraphInitData(PTR graph) {
	char isBezier;
	size_t pointsCount;
	const dreg::Vector2* refs = dreg::getGraphInitData((dreg::Graph*) graph, &isBezier, &pointsCount);
	
	emscripten::val objRefs = emscripten::val::array();
	emscripten::val ret = emscripten::val::object();
	
	for (size_t i = 0; i < pointsCount; i++) {
		emscripten::val point = emscripten::val::array();
		point.set(0, refs[i].x);
		point.set(1, refs[i].y);
		objRefs.set(i, point);
	}
	
	ret.set("isBezier", emscripten::val((bool) isBezier));
	ret.set("refs", objRefs);
	return ret;
}

emscripten::val getGraphPoints(PTR graph) {
	size_t pointsCount;
	const dreg::Vector2* pointsArray = dreg::getGraphPoints((dreg::Graph*) graph, &pointsCount);
	
	emscripten::val xValues = emscripten::val::array();
	emscripten::val yValues = emscripten::val::array();
	
	size_t objArrayPos = 1;
	for (size_t i = 0; i <= pointsCount; i++) {
		xValues.set(objArrayPos, emscripten::val(pointsArray[i].x));
		yValues.set(objArrayPos++, emscripten::val(pointsArray[i].y));
	}
	
	//Add extreme points
	float minX = pointsArray[0].x;
	float maxX = pointsArray[pointsCount - 1].x;
	float interv = (maxX - minX) * 100;
	
	xValues.set(0, emscripten::val(minX - interv));
	yValues.set(0, emscripten::val(dreg::getGraphY((dreg::Graph*) graph, minX - interv)));
	
	xValues.set(pointsCount + 1, emscripten::val(maxX + interv));
	yValues.set(pointsCount + 1, emscripten::val(dreg::getGraphY((dreg::Graph*) graph, maxX + interv)));
	
	emscripten::val ret = emscripten::val::array();
	ret.set(0, xValues);
	ret.set(1, yValues);
	
	return ret;
}

float getGraphY(PTR graph, float x) {
	return dreg::getGraphY((dreg::Graph*) graph, x);
}

EMSCRIPTEN_BINDINGS(drivingEngine) {
	
	emscripten::class_<dreg::Vector2>("Vector2")
		.constructor<>()
		.constructor(&createVector2)
		
		.property("x", &dreg::Vector2::x)
		.property("y", &dreg::Vector2::y)
		;
	
	emscripten::class_<dreg::Vector3>("Vector3")
		.constructor<>()
		.constructor(&createVector3)
		
		.property("x", &dreg::Vector3::x)
		.property("y", &dreg::Vector3::y)
		.property("z", &dreg::Vector3::z)
		;
	
	emscripten::class_<dreg::VehicleControls>("VehicleControls")
		.constructor<>()
		.constructor(&createVehicleControls)
		.property("throttle", &dreg::VehicleControls::throttle)
		.property("brake", &dreg::VehicleControls::brake)
		.property("steeringWheel", &dreg::VehicleControls::steeringWheel)
		.property("clutch", &dreg::VehicleControls::clutch)
		.property("gear", &dreg::VehicleControls::gear)
		;
	
	emscripten::function("updateVehicleStateObj", &updateVehicleStateObj, emscripten::allow_raw_pointers());
	emscripten::function("updateVehiclePropsObj", &updateVehiclePropsObj, emscripten::allow_raw_pointers());
	emscripten::function("updateVehicleConfigStruct", &updateVehicleConfigStruct, emscripten::allow_raw_pointers());
	emscripten::function("updateVehicleConfigObj", &updateVehicleConfigObj, emscripten::allow_raw_pointers());
	
	emscripten::function("createVehicle", &createVehicle, emscripten::allow_raw_pointers());
	emscripten::function("deleteVehicle", &deleteVehicle, emscripten::allow_raw_pointers());
	emscripten::function("resetVehicle", &resetVehicle, emscripten::allow_raw_pointers());
	
	emscripten::function("setVehicleInput", &setVehicleInput, emscripten::allow_raw_pointers());
	emscripten::function("getVehicleState", &getVehicleState, emscripten::allow_raw_pointers());
	emscripten::function("getVehicleProps", &getVehicleProps, emscripten::allow_raw_pointers());
	emscripten::function("getConfigManager", &getConfigManager, emscripten::allow_raw_pointers());
	emscripten::function("setVehicleConfig", &setVehicleConfig, emscripten::allow_raw_pointers());
	emscripten::function("update", &updateVehicle, emscripten::allow_raw_pointers());
	
	emscripten::function("createConfigManager", &createConfigManager, emscripten::allow_raw_pointers());
	emscripten::function("deleteConfigManager", &deleteConfigManager, emscripten::allow_raw_pointers());
	emscripten::function("cloneConfig", &cloneConfig, emscripten::allow_raw_pointers());
	emscripten::function("getVehicleConfig", &getVehicleConfig, emscripten::allow_raw_pointers());
	emscripten::function("updateConfig", &updateConfig, emscripten::allow_raw_pointers());
	emscripten::function("loadDefaultConfig", &loadDefaultConfig, emscripten::allow_raw_pointers());
	emscripten::function("loadSerializedConfig", &loadSerializedConfig, emscripten::allow_raw_pointers());
	emscripten::function("serializeConfig", &serializeConfig, emscripten::allow_raw_pointers());
	
	emscripten::function("setGraphSaveInitData", &dreg::setGraphSaveInitData, emscripten::allow_raw_pointers());
	emscripten::function("setDefaultBezierSamples", &dreg::setDefaultBezierSamples, emscripten::allow_raw_pointers());
	
	emscripten::function("createGraph", &createGraph, emscripten::allow_raw_pointers());
	emscripten::function("deleteGraph", &deleteGraph, emscripten::allow_raw_pointers());
	emscripten::function("cloneGraph", &cloneGraph, emscripten::allow_raw_pointers());
	emscripten::function("loadLinearGraph", &loadLinearGraph, emscripten::allow_raw_pointers());
	emscripten::function("loadBezierGraph", &loadBezierGraph, emscripten::allow_raw_pointers());
	emscripten::function("getGraphInitData", &getGraphInitData, emscripten::allow_raw_pointers());
	emscripten::function("getGraphPoints", &getGraphPoints, emscripten::allow_raw_pointers());
	emscripten::function("getGraphY", &getGraphY, emscripten::allow_raw_pointers());
}