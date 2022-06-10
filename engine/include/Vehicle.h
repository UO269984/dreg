#pragma once

#include "VehicleApi.h"

class Vehicle {
public:
	Vehicle();
	~Vehicle();
	
	void reset();
	void updateConfig();
	void update(float delta);
	
	VehicleConfig config;
	VehicleControls controls;
	VehicleState state;
	VehicleProps props;

private:
	float shaftsDist;
	Vector3 rearShaftPos;
	Vector3 speedVector;
};