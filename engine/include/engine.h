#pragma once

#include "Vehicle.h"

extern "C" {
	typedef void (*PrintFunc)(const char* toPrint);
	extern PrintFunc printFunc;
	void setPrintFunc(PrintFunc newPrintFunc);
	
	Vehicle* createVehicle(VehicleConfig* config);
	void deleteVehicle(Vehicle* vehicle);
	
	void setVehicleInput(Vehicle* vehicle, VehicleControls* input);
	VehicleState* getVehicleState(Vehicle* vehicle);
	void update(Vehicle* vehicle, float delta);
}