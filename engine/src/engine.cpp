#include "engine.h"

#include "Vehicle.h"
#include "InputLogger.h"

#include <cstdio>
#include <fstream>

PrintFunc printFunc = [](const char* toPrint) {printf("- %s\n", toPrint);};
SaveFileFunc saveFileFunc = [](const char* filename, const char* data) {
	std::ofstream f;
	f.open(filename);
	f << data;
	f.close();
};

void setPrintFunc(PrintFunc newPrintFunc) {
	printFunc = newPrintFunc;
}

void setSaveFileFunc(SaveFileFunc newSaveFileFunc) {
	saveFileFunc = newSaveFileFunc;
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

InputLogger* createInputLogger(Vehicle* vehicle) {
	return new InputLogger(vehicle);
}

void logInput(InputLogger* inputLogger, float delta) {
	inputLogger->log(delta);
}

void saveInputLogger(InputLogger* inputLogger, const char* filename) {
	inputLogger->save(filename);
	delete inputLogger;
}