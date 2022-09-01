#pragma once

#include "Vector3.h"

#ifndef __cplusplus
struct Vehicle;
typedef struct Vehicle Vehicle;

struct ConfigManager;
typedef struct ConfigManager ConfigManager;

struct Graph;
typedef struct Graph Graph;

struct InputLogger;
typedef struct InputLogger InputLogger;

typedef void* FloatList;

typedef struct PowerConfig PowerConfig;
typedef struct WheelConfig WheelConfig;
typedef struct VehicleConfig VehicleConfig;
typedef struct VehicleControls VehicleControls;
typedef struct VehicleState VehicleState;
typedef struct VehicleProps VehicleProps;

#else
class Vehicle;
class ConfigManager;
class Graph;
class InputLogger;

#include <vector>
typedef std::vector<float> FloatList;
#endif

struct PowerConfig {
	Graph* throttleCurve; //throttle percent - engine torque multiplier
	Graph* engineCurve; //engine rpm - torque
	Graph* looseEngineRpmCurve; //throttle percent - engine loose rpm
	Graph* engineBrakeCurve; //current rpm minus loose rpm - engine brake torque
	Graph* clutchCurve; //clutch percent - max torque transmission
	
	float torqueToRpmAccel; //Multiplier to convert torque in loose engine to rpm acceleration
	float driveRatio;
	FloatList* gearRatios;
};

struct WheelConfig {
	float diameter;
	
	float brakeDiameter;
	float brakeStaticFrictionCoef;
	float brakeKineticFrictionCoef;
};

struct VehicleConfig {
	//Shafts offsets to vehicle center. They are vectors from vehicle center to each shaft
	Vector3 frontShaft;
	Vector3 rearShaft;
	
	float maxSteeringAngle;
	float mass;
	
	Graph* brakeCurve; //brake percent - brake force
	PowerConfig power;
	WheelConfig wheels;
};

struct VehicleControls {
	float throttle;
	float brake;
	float steeringWheel;
	
	float clutch;
	int gear;
};

struct VehicleState {
	Vector3 pos;
	Vector3 rotation;
};

struct VehicleProps {
	float speed;
	float acceleration;
	
	float wheelRpm;
	float engineRpm;
	float engineTorque;
	float clutchTorque;
	
	float powerTorque;
	float brakeTorque;
	float wheelTorque;
};