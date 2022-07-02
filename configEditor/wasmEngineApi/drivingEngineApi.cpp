#include "drivingEngineApi.h"
#include "updateObj.h"

engine::Vector2 createVector2(float x, float y) {
	return {x, y};
}

engine::Vector3 createVector3(float x, float y, float z) {
	return {x, y, z};
}

engine::VehicleControls createVehicleControls(float throttle, float brake, float steeringWheel, float clutch, int gear) {
	return {throttle, brake, steeringWheel, clutch, gear};
}


PTR createVehicle() {return (PTR) engine::createVehicle();}
void deleteVehicle(PTR vehicle) {engine::deleteVehicle((engine::Vehicle*) vehicle);}
void resetVehicle(PTR vehicle) {engine::resetVehicle((engine::Vehicle*) vehicle);}

void setVehicleInput(PTR vehicle, engine::VehicleControls* vehicleControls) {
	engine::setVehicleInput((engine::Vehicle*) vehicle, vehicleControls);
}

PTR getVehicleState(PTR vehicle) {
	return (PTR) engine::getVehicleState((engine::Vehicle*) vehicle);
}

PTR getVehicleProps(PTR vehicle) {
	return (PTR) engine::getVehicleProps((engine::Vehicle*) vehicle);
}

PTR getVehicleConfig(PTR vehicle) {
	return (PTR) engine::getVehicleConfig((engine::Vehicle*) vehicle);
}

void updateVehicleConfig(PTR vehicle) {
	engine::updateVehicleConfig((engine::Vehicle*) vehicle);
}

void updateVehicle(PTR vehicle, float delta) {
	engine::update((engine::Vehicle*) vehicle, delta);
}


PTR createGraph() {return (PTR) engine::createGraph();}
void deleteGraph(PTR graph) {engine::deleteGraph((engine::Graph*) graph);}

static engine::Vector2* refsToVector2Array(emscripten::val refs) {
	size_t length = refs["length"].as<size_t>();
	engine::Vector2* vector2Array = new engine::Vector2[length];
	
	for (size_t i = 0; i < length; i++)
		vector2Array[i] = {refs[i][0].as<float>(), refs[i][1].as<float>()};
	
	return vector2Array;
}

void loadLinearGraph(PTR graph, emscripten::val refs) {
	engine::Vector2* refsArray = refsToVector2Array(refs);
	engine::loadLinearGraph((engine::Graph*) graph, refsArray, refs["length"].as<size_t>());
	delete[] refsArray;
}

void loadBezierGraph(PTR graph, emscripten::val refs, size_t samplesPerSegment) {
	engine::Vector2* refsArray = refsToVector2Array(refs);
	engine::loadBezierGraph((engine::Graph*) graph, refsArray, refs["length"].as<size_t>(), samplesPerSegment);
	delete[] refsArray;
}

emscripten::val getGraphPoints(PTR graph) {
	size_t pointsCount;
	engine::Vector2* pointsArray = engine::getGraphPoints((engine::Graph*) graph, &pointsCount);
	
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
	yValues.set(0, emscripten::val(engine::getGraphY((engine::Graph*) graph, minX - interv)));
	
	xValues.set(pointsCount + 1, emscripten::val(maxX + interv));
	yValues.set(pointsCount + 1, emscripten::val(engine::getGraphY((engine::Graph*) graph, maxX + interv)));
	
	emscripten::val ret = emscripten::val::array();
	ret.set(0, xValues);
	ret.set(1, yValues);
	
	return ret;
}

float getGraphY(PTR graph, float x) {
	return engine::getGraphY((engine::Graph*) graph, x);
}

EMSCRIPTEN_BINDINGS(drivingEngine) {
	
	emscripten::class_<engine::Vector2>("Vector2")
		.constructor<>()
		.constructor(&createVector2)
		
		.property("x", &engine::Vector2::x)
		.property("y", &engine::Vector2::y)
		;
	
	emscripten::class_<engine::Vector3>("Vector3")
		.constructor<>()
		.constructor(&createVector3)
		
		.property("x", &engine::Vector3::x)
		.property("y", &engine::Vector3::y)
		.property("z", &engine::Vector3::z)
		;
	
	emscripten::class_<engine::VehicleControls>("VehicleControls")
		.constructor<>()
		.constructor(&createVehicleControls)
		.property("throttle", &engine::VehicleControls::throttle)
		.property("brake", &engine::VehicleControls::brake)
		.property("steeringWheel", &engine::VehicleControls::steeringWheel)
		.property("clutch", &engine::VehicleControls::clutch)
		.property("gear", &engine::VehicleControls::gear)
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
	emscripten::function("getVehicleConfig", &getVehicleConfig, emscripten::allow_raw_pointers());
	emscripten::function("updateVehicleConfig", &updateVehicleConfig, emscripten::allow_raw_pointers());
	emscripten::function("update", &updateVehicle, emscripten::allow_raw_pointers());
	
	emscripten::function("createGraph", &createGraph, emscripten::allow_raw_pointers());
	emscripten::function("deleteGraph", &deleteGraph, emscripten::allow_raw_pointers());
	
	emscripten::function("loadLinearGraph", &loadLinearGraph, emscripten::allow_raw_pointers());
	emscripten::function("loadBezierGraph", &loadBezierGraph, emscripten::allow_raw_pointers());
	emscripten::function("getGraphPoints", &getGraphPoints, emscripten::allow_raw_pointers());
	emscripten::function("getGraphY", &getGraphY, emscripten::allow_raw_pointers());
}