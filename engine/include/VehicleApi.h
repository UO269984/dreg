#pragma once

#include "Vector3.h"

#ifndef __cplusplus
struct Vehicle;
typedef struct Vehicle Vehicle;

struct Graph;
typedef struct Graph Graph;

struct InputLogger;
typedef struct InputLogger InputLogger;

typedef struct PowerConfig PowerConfig;
typedef struct WheelConfig WheelConfig;
typedef struct VehicleConfig VehicleConfig;
typedef struct VehicleControls VehicleControls;
typedef struct VehicleState VehicleState;
typedef struct VehicleProps VehicleProps;

#else
class Vehicle;
class Graph;
class InputLogger;
#endif

struct PowerConfig {
	Graph* throttleCurve; //throttle percent - engine torque multiplier
	Graph* engineCurve; //engine rpm - torque
	Graph* looseEngineRpmCurve; //throttle percent - engine loose rpm
	Graph* engineBrakeCurve; //current rpm minus loose rpm - engine brake torque
	Graph* clutchCurve; //clutch percent - max torque transmission
	
	float torqueToRpmAccel; //Multiplier to convert torque in loose engine to rpm acceleration
	float driveRatio;
	int gearsCount;
	float* gearRatios;
};

struct WheelConfig {
	float diameter;
};

struct VehicleConfig {
	//Shafts offsets to vehicle center. They are vectors from vehicle center to each shaft
	Vector3 frontShaft;
	Vector3 rearShaft;
	
	float maxSteeringAngle;
	float mass;
	
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
	float axleShaftTorque;
};