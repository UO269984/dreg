#pragma once

#include "Vector3.h"

struct VehicleConfig {
	//Shafts offset to vehicle center
	Vector3 frontShaft;
	Vector3 rearShaft;
	
	float maxSteeringAngle;
};

struct VehicleControls {
	float throttle;
	float brake;
	float steeringWheel;
};

struct VehicleState {
	Vector3 pos;
	Vector3 rotation;
};

class Vehicle {
public:
	Vehicle(VehicleConfig config);
	~Vehicle();
	
	void update(float delta);
	
	VehicleControls controls = {0};
	VehicleState state = {0};

private:
	Vector3 rearShaftPos = {0};
	
	Vector3 speedVector = {1, 0, 0};
	float speed = 0;
	float acceleration = 15;
	
	float shaftsDist;
	VehicleConfig config;
};