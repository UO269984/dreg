#include "Vehicle.h"

#include "dreg.h"
#include "ConfigManager.h"
#include "Graph.h"

Vehicle::Vehicle() {
	printFunc("Creating Vehicle");
	reset();
}

Vehicle::~Vehicle() {
	printFunc("Destroying Vehicle");
	clearConfig();
}

void Vehicle::reset() {
	controls = {0, 0, 0, 0, 0};
	state = {{0, 0, 0}, {0, 0, 0}};
	props = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	speedVector = {1, 0, 0};
	if (configManager != NULL)
		updateConfig();
}

void Vehicle::setConfig(ConfigManager* newConfigManager) {
	clearConfig();
	
	if (newConfigManager != NULL) {
		config = &newConfigManager->config;
		configManager = newConfigManager;
		
		newConfigManager->addVehicle(this);
	}
}

void Vehicle::clearConfig() {
	if (configManager != NULL) {
		configManager->removeVehicle(this);
		configManager = NULL;
	}
}

void Vehicle::updateConfig() {
	shaftsDist = (config->frontShaft - config->rearShaft).length();
	rearShaftPos = state.pos + config->rearShaft;
	wheelPerimeter = M_PI * config->wheels.diameter;
}

void Vehicle::update(float delta) {
	float prevSpeed = props.speed;
	props.speed += props.acceleration * delta;
	
	if ((prevSpeed > 0) != (props.speed > 0) && fabs(props.brakeTorque) > fabs(props.powerTorque))
		props.speed = 0;
	
	props.wheelRpm = (props.speed / wheelPerimeter) * 60;
	
	props.powerTorque = 0;
	props.brakeTorque = 0;
	updatePower(delta);
	updateBreaks();
	
	props.wheelTorque = props.powerTorque + props.brakeTorque;
	props.acceleration = (props.wheelTorque / (config->wheels.diameter / 2)) / config->mass;
	
	if ((controls.steeringWheel > 0.1 || controls.steeringWheel < 0.1) && props.speed != 0) {
		float rotTime = shaftsDist / props.speed;
		float rotateDegrees = config->maxSteeringAngle * controls.steeringWheel * (delta / rotTime);
		
		speedVector = speedVector.rotateEuler(0, 0, -rotateDegrees);
		state.rotation.z += rotateDegrees;
	}
	
	rearShaftPos += speedVector * (props.speed * delta);
	state.pos = rearShaftPos - config->rearShaft.rotateEuler(0, 0, -state.rotation.z);
}

void Vehicle::updatePower(float delta) {
	//Update engine rpm
	float transmissionRatio = (*config->power.gearRatios)[controls.gear] * config->power.driveRatio;
	
	float clutchRatio;
	if (transmissionRatio == 0) {
		clutchRatio = 0;
		props.clutchTorque = 0;
	}
	else {
		clutchRatio = props.engineTorque == 0 ? 0 :
			props.clutchTorque / props.engineTorque;
	}
	float looseEngineRpm = props.engineRpm +
		(props.engineTorque - props.clutchTorque) * config->power.torqueToRpmAccel * delta;
	
	props.engineRpm = clutchRatio * props.wheelRpm * transmissionRatio + (1 - clutchRatio) * looseEngineRpm;
	
	//Compute the new torque of the axle shaft
	float throttle = config->power.throttleCurve->getY(controls.throttle);
	float expectedLooseRpm = config->power.looseEngineRpmCurve->getY(throttle);
	bool engineBrakeActive = props.engineRpm > expectedLooseRpm;
	
	props.engineTorque = engineBrakeActive ?
		-config->power.engineBrakeCurve->getY(props.engineRpm - expectedLooseRpm) :
		config->power.engineCurve->getY(props.engineRpm) * throttle;
	
	float clutchMaxTorque = config->power.clutchCurve->getY(controls.clutch);
	props.clutchTorque = fmin(fabs(props.engineTorque), clutchMaxTorque);
	if (props.engineTorque < 0)
		props.clutchTorque = -props.clutchTorque;
	
	(engineBrakeActive ? props.brakeTorque : props.powerTorque) += props.clutchTorque * transmissionRatio;
}

void Vehicle::updateBreaks() {
	bool wheelStopped = props.wheelRpm == 0;
	
	float frictionCoef = wheelStopped ?
		config->wheels.brakeStaticFrictionCoef : config->wheels.brakeKineticFrictionCoef;
	
	float brakeTorque = config->brakeCurve->getY(controls.brake) *
		frictionCoef * (config->wheels.brakeDiameter / 2);
	
	if (wheelStopped)
		props.brakeTorque += fmin(brakeTorque, fabs(props.powerTorque)) * (props.powerTorque < 0 ? 1 : -1);
	
	else
		props.brakeTorque += props.wheelRpm > 0 ? -brakeTorque : brakeTorque;
}