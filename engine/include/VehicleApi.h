#pragma once

#include "Vector3.h"

struct VehicleConfig {
	//Shafts offsets to vehicle center. They are vectors from vehicle center to each shaft
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

struct VehicleProps {
	float speed;
	float acceleration;
};

#ifndef __cplusplus
struct Vehicle;
typedef struct Vehicle Vehicle;

struct Graph;
typedef struct Graph Graph;

struct InputLogger;
typedef struct InputLogger InputLogger;

typedef struct VehicleConfig VehicleConfig;
typedef struct VehicleControls VehicleControls;
typedef struct VehicleState VehicleState;
typedef struct VehicleProps VehicleProps;

#else
class Vehicle;
class Graph;
class InputLogger;
#endif