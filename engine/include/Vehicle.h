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
	void updatePower(float delta);
	
	float shaftsDist;
	Vector3 rearShaftPos;
	Vector3 speedVector;
	
	float wheelPerimeter;
	bool connectedClutch; //If the clutch is fully engaged
};