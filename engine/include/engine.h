#pragma once

#include "VehicleApi.h"

#ifdef __cplusplus
extern "C" {
#endif
	
	typedef void (*PrintFunc)(const char* toPrint);
	extern PrintFunc printFunc;
	void setPrintFunc(PrintFunc newPrintFunc);
	
	Vehicle* createVehicle(VehicleConfig* config);
	void deleteVehicle(Vehicle* vehicle);
	
	void setVehicleInput(Vehicle* vehicle, VehicleControls* input);
	VehicleState* getVehicleState(Vehicle* vehicle);
	void update(Vehicle* vehicle, float delta);

#ifdef __cplusplus
}
#endif