#include <emscripten.h>
#include <emscripten/bind.h>

namespace engine {
	#include "engine.h"
}

#define PTR intptr_t

engine::Vector2 createVector2(float x, float y) {
	return {x, y};
}

engine::Vector3 createVector3(float x, float y, float z) {
	return {x, y, z};
}

engine::VehicleConfig createVehicleConfig(engine::Vector3 frontShaft, engine::Vector3 rearShaft, float maxSteeringAngle) {
	return {frontShaft, rearShaft, maxSteeringAngle};
}

engine::VehicleControls createVehicleControls(float throttle, float brake, float steeringWheel) {
	return {throttle, brake, steeringWheel};
}


void updateVector3Struct(engine::Vector3* vector, emscripten::val obj) {
	vector->x = obj["x"].as<float>();
	vector->y = obj["y"].as<float>();
	vector->z = obj["z"].as<float>();
}

void updateVector3Obj(engine::Vector3* vector, emscripten::val obj) {
	obj.set("x", emscripten::val(vector->x));
	obj.set("y", emscripten::val(vector->y));
	obj.set("z", emscripten::val(vector->z));
}

void updateVehicleStateObj(engine::VehicleState* state, emscripten::val obj) {
	updateVector3Obj(&state->pos, obj["pos"]);
	updateVector3Obj(&state->rotation, obj["rotation"]);
}

void updateVehiclePropsObj(engine::VehicleProps* props, emscripten::val obj) {
	obj.set("speed", emscripten::val(props->speed));
	obj.set("acceleration", emscripten::val(props->acceleration));
}

void updateVehicleConfigStruct(engine::VehicleConfig* config, emscripten::val obj) {
	updateVector3Struct(&config->frontShaft, obj["frontShaft"]);
	updateVector3Struct(&config->rearShaft, obj["rearShaft"]);
	config->maxSteeringAngle = obj["maxSteeringAngle"].as<float>();
}

void updateVehicleConfigObj(engine::VehicleConfig* config, emscripten::val obj) {
	updateVector3Obj(&config->frontShaft, obj["frontShaft"]);
	updateVector3Obj(&config->rearShaft, obj["rearShaft"]);
	obj.set("maxSteeringAngle", emscripten::val(config->maxSteeringAngle));
}


PTR createVehicle() {return (PTR) engine::createVehicle();}
void deleteVehicle(PTR vehicle) {engine::deleteVehicle((engine::Vehicle*) vehicle);}
void resetVehicle(PTR vehicle) {engine::resetVehicle((engine::Vehicle*) vehicle);}

void setVehicleInput(PTR vehicle, engine::VehicleControls* vehicleControls) {
	engine::setVehicleInput((engine::Vehicle*) vehicle, vehicleControls);
}

engine::VehicleState* getVehicleState(PTR vehicle) {
	return engine::getVehicleState((engine::Vehicle*) vehicle);
}

engine::VehicleProps* getVehicleProps(PTR vehicle) {
	return engine::getVehicleProps((engine::Vehicle*) vehicle);
}

engine::VehicleConfig* getVehicleConfig(PTR vehicle) {
	return engine::getVehicleConfig((engine::Vehicle*) vehicle);
}

void updateVehicleConfig(PTR vehicle) {
	engine::updateVehicleConfig((engine::Vehicle*) vehicle);
}

void updateVehicle(PTR vehicle, float delta) {
	engine::update((engine::Vehicle*) vehicle, delta);
}


PTR createGraph() {return (PTR) engine::createGraph();}
void deleteGraph(PTR graph) {engine::deleteGraph((engine::Graph*) graph);}

void loadLinearGraph(PTR graph, PTR refs, size_t refsCount, size_t samplesPerSegment) {
	engine::loadLinearGraph((engine::Graph*) graph, (engine::Vector2*) refs, refsCount);
}

void loadBezierGraph(PTR graph, PTR refs, size_t refsCount, size_t samplesPerSegment) {
	engine::loadBezierGraph((engine::Graph*) graph, (engine::Vector2*) refs, refsCount, samplesPerSegment);
}

engine::Vector2* getGraphPoints(PTR graph, PTR pointsCount) {
	return engine::getGraphPoints((engine::Graph*) graph, (size_t*) pointsCount);
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
	
	emscripten::class_<engine::VehicleConfig>("VehicleConfig")
		.constructor<>()
		.constructor(&createVehicleConfig)
		
		.property("frontShaft", &engine::VehicleConfig::frontShaft)
		.property("rearShaft", &engine::VehicleConfig::rearShaft)
		.property("maxSteeringAngle", &engine::VehicleConfig::maxSteeringAngle)
		;
	
	emscripten::class_<engine::VehicleControls>("VehicleControls")
		.constructor<>()
		.constructor(&createVehicleControls)
		.property("throttle", &engine::VehicleControls::throttle)
		.property("brake", &engine::VehicleControls::brake)
		.property("steeringWheel", &engine::VehicleControls::steeringWheel)
		;
	
	emscripten::class_<engine::VehicleState>("VehicleState")
		.constructor<>()
		.property("pos", &engine::VehicleState::pos)
		.property("rotation", &engine::VehicleState::rotation)
		;
	
	emscripten::class_<engine::VehicleProps>("VehicleProps")
		.constructor<>()
		.property("speed", &engine::VehicleProps::speed)
		.property("acceleration", &engine::VehicleProps::acceleration)
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