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

#ifndef __cplusplus
struct Vehicle;
typedef struct Vehicle Vehicle;

struct InputLogger;
typedef struct InputLogger InputLogger;

typedef struct VehicleConfig VehicleConfig;
typedef struct VehicleControls VehicleControls;
typedef struct VehicleState VehicleState;

#else
class Vehicle;
class InputLogger;
#endif