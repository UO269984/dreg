#include "engine.h"

#include <iostream>
using namespace std;

void test(Vehicle* vehicle, int iters) {
	VehicleControls controls = {0.5, 0, 1};
	setVehicleInput(vehicle, &controls);
	VehicleState* state = getVehicleState(vehicle);
	
	for (int i = 0; i < iters; i++) {
		update(vehicle, 0.1);
		cout << i + 1 << " " << state->pos.x << ", " << state->pos.y << ", " << state->pos.z << endl;
		cout << "\trot z: " << state->rotation.z << "º" << endl;
	}
}

void testInputLogger(Vehicle* vehicle) {
	//setSaveFileFunc([](const char* filename, const char* data) {cout << "Saving file: " << filename << endl;});
	
	InputLogger* inputLogger = createInputLogger(vehicle);
	VehicleControls controls = {0.1, 0, 0};
	
	setVehicleInput(vehicle, &controls);
	logInput(inputLogger, 0.1); //Delta must be ignored in first log
	logInput(inputLogger, 0.2);
	
	setVehicleInput(vehicle, &controls);
	logInput(inputLogger, 0.2);
	
	controls.brake = 0.5;
	setVehicleInput(vehicle, &controls);
	logInput(inputLogger, 0.1);
	
	controls.throttle = 0;
	setVehicleInput(vehicle, &controls);
	logInput(inputLogger, 0.1);
	
	controls.steeringWheel = 0.3;
	setVehicleInput(vehicle, &controls);
	logInput(inputLogger, 0.1);
	saveInputLogger(inputLogger, "../../VehicleInputTest.csv");
}

int main(int argc, char const *argv[]) {
	int updateItersCount = argc > 1 ? atoi(argv[1]) : 10;
	
	VehicleConfig config = {{0, 0, 0}, {1, 0, 0}, 40.0f};
	Vehicle* vehicle = createVehicle(&config);
	test(vehicle, updateItersCount);
	
	testInputLogger(vehicle);
	deleteVehicle(vehicle);
	cout << "End test" << endl;
}