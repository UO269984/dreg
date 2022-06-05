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

void testGraph() {
	Vector2 refs[] = {{0, 0}, {0.1, 0.2}, {0.6, 0.8}, {1, 1}};
	
	Graph* graph = createGraph();
	cout << "Linear graph" << endl;
	loadLinearGraph(graph, refs, 4);
	
	int i = 0;
	for (float x = 0; x < 1; x += 0.01) {
		cout << x << " -> " << getGraphY(graph, x) << "   ";
		
		if (i++ % 2 == 0)
			cout << endl;
	}
	cout << endl << "Bezier graph" << endl;
	
	Vector2 bezierRefs[] = {{0, 0}, {0.1, 0.1}, {0.2, 0.2}, {0.3, 0.3},
		{0.4, 0.4}, {0.5, 0.5}, {0.6, 0.6}};
	
	loadBezierGraph(graph, refs, 7, 20);
	for (float x = 0; x < 1; x += 0.01) {
		cout << x << " -> " << getGraphY(graph, x) << "   ";
		
		if (i++ % 2 == 0)
			cout << endl;
	}
	cout << endl;
	deleteGraph(graph);
}

void (*printTest)(const char* testName) = [](const char* testName) {cout << endl << "TEST: " << testName << endl;};

int main(int argc, char const *argv[]) {
	int updateItersCount = argc > 1 ? atoi(argv[1]) : 10;
	
	printTest("Vehicle");
	VehicleConfig config = {{0, 0, 0}, {1, 0, 0}, 40.0f};
	Vehicle* vehicle = createVehicle(&config);
	test(vehicle, updateItersCount);
	
	printTest("Input logger");
	testInputLogger(vehicle);
	deleteVehicle(vehicle);
	
	printTest("Graph");
	testGraph();
	cout << "End test" << endl;
}