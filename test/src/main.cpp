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

int main(int argc, char const *argv[]) {
	int updateItersCount = argc > 1 ? atoi(argv[1]) : 10;
	
	VehicleConfig config = {{0, 0, 0}, {1, 0, 0}, 40.0f};
	Vehicle* vehicle = createVehicle(&config);
	test(vehicle, updateItersCount);
	
	deleteVehicle(vehicle);
	cout << "End test" << endl;
}