#include "dreg/dreg.h"

#include <functional>
#include <iostream>
using namespace std;

void loadConfig(Vehicle* vehicle) {
	VehicleConfig* config = getVehicleConfig(getConfigManager(vehicle));
	config->maxSteeringAngle = 40.0f;
	updateConfig(getConfigManager(vehicle));
	
	Vector2 refs[] = {{0, 0}, {1, 3000}};
	loadLinearGraph(config->power.looseEngineRpmCurve, refs, 2);
	
	refs[0] = {0, 80};
	refs[1] = {1000, 1500};
	loadLinearGraph(config->power.engineCurve, refs, 2);
	
	refs[0] = {200, 0};
	refs[1] = {1000, -100};
	loadLinearGraph(config->power.engineBrakeCurve, refs, 2);
	
	refs[0] = {0, 5000};
	refs[1] = {0, 0};
	loadLinearGraph(config->power.clutchCurve, refs, 2);
}

void testVehicle(Vehicle* vehicle, int iters) {
	VehicleControls controls = {0.5, 0, 1, 0, 2};
	setVehicleInput(vehicle, &controls);
	const VehicleState* state = getVehicleState(vehicle);
	const VehicleProps* vehicleProps = getVehicleProps(vehicle);
	
	function<void()> printVehicle = [state, vehicleProps]() {
		cout << state->pos.x << ", " << state->pos.y << ", " << state->pos.z << endl;
		cout << "\trot z: " << state->rotation.z << "º" << " Speed: " << vehicleProps->speed << " m/s" << endl;
	};
	
	for (int i = 0; i < iters; i++) {
		update(vehicle, 0.1);
		cout << i + 1 << " ";
		printVehicle();
	}
	
	cout << "Resetting vehicle" << endl;
	resetVehicle(vehicle);
	setVehicleInput(vehicle, &controls);
	printVehicle();
	update(vehicle, 0.1);
	printVehicle();
	
	cout << "Updating vehicle config" << endl;
	getVehicleConfig(getConfigManager(vehicle))->rearShaft.x = 1.5;
	updateConfig(getConfigManager(vehicle));
	update(vehicle, 0.1);
	printVehicle();
}

void testConfigManager(ConfigManager* configManager) {
	char* serialized = serializeConfig(getVehicleConfig(configManager));
	cout << "Serialized config:" << endl << serialized << "\n\n";
	
	ConfigManager* configManager2 = createConfigManager(1);
	loadSerializedConfig(getVehicleConfig(configManager2), serialized);
	deleteCharArray(serialized);
	
	serialized = serializeConfig(getVehicleConfig(configManager2));
	cout << "Deserialized - Serialized config:" << endl << serialized << endl;
	
	deleteConfigManager(configManager2);
	deleteCharArray(serialized);
}

void testInputLogger(Vehicle* vehicle) {
	//setSaveFileFunc([](const char* filename, const char* data) {cout << "Saving file: " << filename << endl;});
	
	InputLogger* inputLogger = createInputLogger(vehicle);
	VehicleControls controls = {0.1, 0, 0, 0, 2};
	
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
	for (float x = 0; x < 1; x += 0.1) {
		cout << x << " -> " << getGraphY(graph, x) << "   ";
		
		if (i++ % 2 == 0)
			cout << endl;
	}
	cout << endl << "Bezier graph" << endl;
	
	Vector2 bezierRefs[] = {{0, 0}, {0.1, 0.1}, {0.2, 0.2}, {0.3, 0.3},
		{0.4, 0.4}, {0.5, 0.5}, {0.6, 0.6}};
	
	loadBezierGraph(graph, bezierRefs, 7, 20);
	for (float x = 0; x < 1; x += 0.1) {
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
	
	setGraphSaveInitData(1);
	printTest("Vehicle");
	Vehicle* vehicle = createVehicle();
	ConfigManager* configManager = createConfigManager(1);
	
	loadDefaultConfig(configManager);
	setVehicleConfig(vehicle, configManager);
	loadConfig(vehicle);
	testVehicle(vehicle, updateItersCount);
	
	printTest("Input ConfigManager");
	testConfigManager(configManager);
	
	printTest("Input logger");
	testInputLogger(vehicle);
	deleteConfigManager(configManager);
	
	printTest("Graph");
	testGraph();
	cout << "End test" << endl;
}