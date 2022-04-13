#include "engine.h"

#include <cstdio>

PrintFunc printFunc = [](const char* toPrint) {printf("- %s\n", toPrint);};

void setPrintFunc(PrintFunc newPrintFunc) {
	printFunc = newPrintFunc;
}

Vehicle* createVehicle(VehicleConfig* config) {
	return new Vehicle(*config);
}

void deleteVehicle(Vehicle* vehicle) {
	delete vehicle;
}

void setVehicleInput(Vehicle* vehicle, VehicleControls* input) {
	vehicle->controls = *input;
}

VehicleState* getVehicleState(Vehicle* vehicle) {
	return &vehicle->state;
}

void update(Vehicle* vehicle, float delta) {
	vehicle->update(delta);
}