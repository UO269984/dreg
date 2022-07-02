#include "updateObj.h"

static void updateVector3Struct(engine::Vector3* vector, emscripten::val obj) {
	vector->x = obj["x"].as<float>();
	vector->y = obj["y"].as<float>();
	vector->z = obj["z"].as<float>();
}

static void updateVector3Obj(engine::Vector3* vector, emscripten::val obj) {
	obj.set("x", emscripten::val(vector->x));
	obj.set("y", emscripten::val(vector->y));
	obj.set("z", emscripten::val(vector->z));
}

void updateVehicleStateObj(PTR statePtr, emscripten::val obj) {
	engine::VehicleState* state = (engine::VehicleState*) statePtr;
	
	updateVector3Obj(&state->pos, obj["pos"]);
	updateVector3Obj(&state->rotation, obj["rotation"]);
}

void updateVehiclePropsObj(PTR propsPtr, emscripten::val obj) {
	engine::VehicleProps* props = (engine::VehicleProps*) propsPtr;
	
	obj.set("speed", emscripten::val(props->speed));
	obj.set("acceleration", emscripten::val(props->acceleration));
	
	obj.set("wheelRpm", emscripten::val(props->wheelRpm));
	obj.set("engineRpm", emscripten::val(props->engineRpm));
	obj.set("engineTorque", emscripten::val(props->engineTorque));
	obj.set("clutchTorque", emscripten::val(props->clutchTorque));
	obj.set("axleShaftTorque", emscripten::val(props->axleShaftTorque));
}

static void updateFloatArrayObj(float* array, emscripten::val obj, int length) {
	for (int i = 0; i < length; i++)
		obj.set(i, emscripten::val(array[i]));
}

static void updateFloatArrayPtr(float* array, emscripten::val obj, int length) {
	for (int i = 0; i < length; i++)
		array[i] = obj[i].as<float>();
}

static void updatePowerConfigObj(engine::PowerConfig* powerConfig, emscripten::val obj) {
	obj.set("throttleCurve", emscripten::val((PTR) powerConfig->throttleCurve));
	obj.set("engineCurve", emscripten::val((PTR) powerConfig->engineCurve));
	obj.set("looseEngineRpmCurve", emscripten::val((PTR) powerConfig->looseEngineRpmCurve));
	obj.set("engineBrakeCurve", emscripten::val((PTR) powerConfig->engineBrakeCurve));
	obj.set("clutchCurve", emscripten::val((PTR) powerConfig->clutchCurve));
	
	obj.set("torqueToRpmAccel", emscripten::val(powerConfig->torqueToRpmAccel));
	obj.set("driveRatio", emscripten::val(powerConfig->driveRatio));
	obj.set("gearsCount", emscripten::val(powerConfig->gearsCount));
	
	emscripten::val gearsObj = emscripten::val::array();
	updateFloatArrayObj(powerConfig->gearRatios, gearsObj, powerConfig->gearsCount);
	obj.set("gearRatios", gearsObj);
}

static void updatePowerConfigStruct(engine::PowerConfig* powerConfig, emscripten::val obj) {
	powerConfig->throttleCurve = (engine::Graph*) obj["throttleCurve"].as<PTR>();
	powerConfig->engineCurve = (engine::Graph*) obj["engineCurve"].as<PTR>();
	powerConfig->looseEngineRpmCurve = (engine::Graph*) obj["looseEngineRpmCurve"].as<PTR>();
	powerConfig->engineBrakeCurve = (engine::Graph*) obj["engineBrakeCurve"].as<PTR>();
	powerConfig->clutchCurve = (engine::Graph*) obj["clutchCurve"].as<PTR>();
	
	powerConfig->torqueToRpmAccel = obj["torqueToRpmAccel"].as<float>();
	powerConfig->driveRatio = obj["driveRatio"].as<float>();
	powerConfig->gearsCount = obj["gearsCount"].as<int>();
	
	engine::deleteFloatArray(powerConfig->gearRatios);
	powerConfig->gearsCount = obj["gearRatios"]["length"].as<int>();
	powerConfig->gearRatios = engine::createFloatArray(powerConfig->gearsCount);
	updateFloatArrayPtr(powerConfig->gearRatios, obj["gearRatios"], powerConfig->gearsCount);
}

static void updateWheelConfigObj(engine::WheelConfig* wheelConfig, emscripten::val obj) {
	obj.set("diameter", emscripten::val(wheelConfig->diameter));
}

static void updateWheelConfigStruct(engine::WheelConfig* wheelConfig, emscripten::val obj) {
	wheelConfig->diameter = obj["diameter"].as<float>();
}

void updateVehicleConfigStruct(PTR configPtr, emscripten::val obj) {
	engine::VehicleConfig* config = (engine::VehicleConfig*) configPtr;
	
	updateVector3Struct(&config->frontShaft, obj["frontShaft"]);
	updateVector3Struct(&config->rearShaft, obj["rearShaft"]);
	config->maxSteeringAngle = obj["maxSteeringAngle"].as<float>();
	config->maxSteeringAngle = obj["mass"].as<float>();
	
	updatePowerConfigStruct(&config->power, obj["power"]);
	updateWheelConfigStruct(&config->wheels, obj["wheels"]);
}

void updateVehicleConfigObj(PTR configPtr, emscripten::val obj) {
	engine::VehicleConfig* config = (engine::VehicleConfig*) configPtr;
	
	updateVector3Obj(&config->frontShaft, obj["frontShaft"]);
	updateVector3Obj(&config->rearShaft, obj["rearShaft"]);
	obj.set("maxSteeringAngle", emscripten::val(config->maxSteeringAngle));
	obj.set("mass", emscripten::val(config->mass));
	
	updatePowerConfigObj(&config->power, obj["power"]);
	updateWheelConfigObj(&config->wheels, obj["wheels"]);
}