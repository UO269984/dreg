#include "Vehicle.h"

#include "engine.h"

Vehicle::Vehicle() : config({{0, 0, 0}, {2, 0, 0}, 20.0f}) {
	reset();
	printFunc("Creating vehicle");
}

Vehicle::~Vehicle() {
	printFunc("Destroying vehicle");
}

void Vehicle::reset() {
	controls = {0, 0, 0};
	state = {{0, 0, 0}, {0, 0, 0}};
	props = {0, 15};
	
	speedVector = {1, 0, 0};
	updateConfig();
}

void Vehicle::updateConfig() {
	shaftsDist = (config.frontShaft - config.rearShaft).length();
	rearShaftPos = state.pos + config.rearShaft;
}

void Vehicle::update(float delta) {
	if (controls.throttle > 0.001)
		props.speed += props.acceleration * controls.throttle * delta;
	
	if (controls.brake > 0.001)
		props.speed -= props.acceleration * controls.brake * delta;
	
	if (controls.steeringWheel > 0.1 || controls.steeringWheel < 0.1) {
		float rotTime = shaftsDist / props.speed;
		float rotateDegrees = config.maxSteeringAngle * controls.steeringWheel * (delta / rotTime);
		
		speedVector = speedVector.rotateEuler(0, 0, -rotateDegrees);
		state.rotation.z += rotateDegrees;
	}
	
	rearShaftPos += speedVector * (props.speed * delta);
	state.pos = rearShaftPos - config.rearShaft.rotateEuler(0, 0, -state.rotation.z);
}