#include "Vehicle.h"

#include "engine.h"

Vehicle::Vehicle(VehicleConfig config) :
	config(config), shaftsDist((config.frontShaft - config.rearShaft).length()) {
	
	printFunc("Creating vehicle");
}

Vehicle::~Vehicle() {
	printFunc("Destroying vehicle");
}

void Vehicle::update(float delta) {
	if (controls.throttle > 0.001)
		speed += acceleration * controls.throttle * delta;
	
	if (controls.brake > 0.001)
		speed -= acceleration * controls.brake * delta;
	
	if (controls.steeringWheel > 0.1 || controls.steeringWheel < 0.1) {
		float rotTime = shaftsDist / speed;
		float rotateDegrees = config.maxSteeringAngle * controls.steeringWheel * (delta / rotTime);
		
		speedVector = speedVector.rotateEuler(0, 0, -rotateDegrees);
		state.rotation.z += rotateDegrees;
	}
	
	rearShaftPos += speedVector * (speed * delta);
	state.pos = rearShaftPos - config.rearShaft.rotateEuler(0, 0, -state.rotation.z);
}