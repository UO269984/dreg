#include "Vehicle.h"

#include "dreg.h"
#include "Graph.h"

Vehicle::Vehicle() : config({{0, 0, 0}, {2, 0, 0}, 20, 500,
		{new Graph(), new Graph(), new Graph(), new Graph(), new Graph(), 1, 1, 1, new float[1] {1}},
		{0.5}}) {
	
	Vector2 refs[] = {{0, 0}, {1, 1}};
	config.power.throttleCurve->loadLinear(refs, 2);
	config.power.engineCurve->loadLinear(refs, 2);
	
	refs[1].y = 0;
	config.power.looseEngineRpmCurve->loadLinear(refs, 2);
	config.power.engineBrakeCurve->loadLinear(refs, 2);
	
	refs[0] = {0, 1};
	refs[1] = {1, 0};
	config.power.clutchCurve->loadLinear(refs, 2);
	
	reset();
	printFunc("Creating vehicle");
}

Vehicle::~Vehicle() {
	delete config.power.throttleCurve;
	delete config.power.engineCurve;
	delete config.power.looseEngineRpmCurve;
	delete config.power.engineBrakeCurve;
	delete config.power.clutchCurve;
	
	delete[] config.power.gearRatios;
	
	printFunc("Destroying vehicle");
}

void Vehicle::reset() {
	controls = {0, 0, 0, 0, 0};
	state = {{0, 0, 0}, {0, 0, 0}};
	props = {0, 0, 0, 0, 0, 0, 0};
	
	speedVector = {1, 0, 0};
	connectedClutch = true;
	updateConfig();
}

void Vehicle::updateConfig() {
	shaftsDist = (config.frontShaft - config.rearShaft).length();
	rearShaftPos = state.pos + config.rearShaft;
	wheelPerimeter = M_PI * config.wheels.diameter;
}

void Vehicle::update(float delta) {
	props.speed += props.acceleration * delta;
	props.wheelRpm = (props.speed / wheelPerimeter) * 60;
	updatePower(delta);
	props.acceleration = (props.axleShaftTorque / (config.wheels.diameter / 2)) / config.mass;
	
	if ((controls.steeringWheel > 0.1 || controls.steeringWheel < 0.1) && props.speed != 0) {
		float rotTime = shaftsDist / props.speed;
		float rotateDegrees = config.maxSteeringAngle * controls.steeringWheel * (delta / rotTime);
		
		speedVector = speedVector.rotateEuler(0, 0, -rotateDegrees);
		state.rotation.z += rotateDegrees;
	}
	
	rearShaftPos += speedVector * (props.speed * delta);
	state.pos = rearShaftPos - config.rearShaft.rotateEuler(0, 0, -state.rotation.z);
}

void Vehicle::updatePower(float delta) {
	//Update engine rpm
	float transmissionRatio = config.power.gearRatios[controls.gear] * config.power.driveRatio;
	
	float clutchRatio;
	if (transmissionRatio == 0) {
		clutchRatio = 0;
		props.clutchTorque = 0;
	}
	else {
		clutchRatio = connectedClutch ? 1 :
			(props.engineTorque == 0 ? 0 :
			props.clutchTorque / props.engineTorque);
	}
	float looseEngineRpm = 0;
	if (clutchRatio != 1) {
		looseEngineRpm = (1 - clutchRatio) * (props.engineRpm +
			(props.engineTorque - props.clutchTorque) * config.power.torqueToRpmAccel * delta);
	}
	props.engineRpm = clutchRatio * props.wheelRpm * transmissionRatio + looseEngineRpm;
	
	//Compute the new torque of the axle shaft
	float throttle = config.power.throttleCurve->getY(controls.throttle);
	float expectedLooseRpm = config.power.looseEngineRpmCurve->getY(throttle);
	props.engineTorque = config.power.engineCurve->getY(props.engineRpm) * throttle;
	
	if (props.engineRpm > expectedLooseRpm)
		props.engineTorque += config.power.engineBrakeCurve->getY(props.engineRpm - expectedLooseRpm);
	
	float clutchMaxTorque = config.power.clutchCurve->getY(controls.clutch);
	connectedClutch = abs(props.engineTorque) < clutchMaxTorque;
	
	props.clutchTorque = connectedClutch ?
		props.engineTorque :
		(props.engineTorque < 0 ? -clutchMaxTorque : clutchMaxTorque);
	
	props.axleShaftTorque = props.clutchTorque * transmissionRatio;
}