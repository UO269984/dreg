#include "engine.h"

#include "Vehicle.h"
#include "Graph.h"
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


Graph* createGraph() {
	return new Graph();
}

void deleteGraph(Graph* graph) {
	delete graph;
}

void loadLinearGraph(Graph* graph, Vector2* refs, size_t refsCount) {
	graph->loadLinear(refs, refsCount);
}

void loadBezierGraph(Graph* graph, Vector2* refs, size_t refsCount, size_t samplesPerSegment) {
	graph->loadBezier(refs, refsCount, samplesPerSegment);
}

Vector2* getGraphPoints(Graph* graph, size_t* pointsCount) {
	return graph->getPoints(pointsCount);
}

float getGraphY(Graph* graph, float x) {
	return graph->getY(x);
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