#pragma once

#include "VehicleApi.h"

class Vehicle {
public:
	Vehicle();
	~Vehicle();
	
	void reset();
	void setConfig(ConfigManager* newConfigManager);
	void updateConfig();
	void update(float delta);
	
	ConfigManager* configManager = NULL;
	VehicleControls controls;
	VehicleState state;
	VehicleProps props;

private:
	void removeVehicleFromConfig();
	
	void updatePower(float delta);
	void updateBreaks();
	
	VehicleConfig* config;
	
	float shaftsDist;
	Vector3 rearShaftPos;
	Vector3 speedVector;
	
	float wheelPerimeter;
};