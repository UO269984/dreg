#pragma once

#include "VehicleApi.h"

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