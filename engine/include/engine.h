#pragma once

#include "VehicleApi.h"
#include "Vector2.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
	
	typedef void (*PrintFunc)(const char* toPrint);
	typedef void (*SaveFileFunc)(const char* filename, const char* data);
	
	extern PrintFunc printFunc;
	void setPrintFunc(PrintFunc newPrintFunc);
	
	extern SaveFileFunc saveFileFunc;
	void setSaveFileFunc(SaveFileFunc newSaveFileFunc);
	
	float* createFloatArray(size_t size);
	void deleteFloatArray(float* array);
	
	Vehicle* createVehicle();
	void deleteVehicle(Vehicle* vehicle);
	void resetVehicle(Vehicle* vehicle);
	
	void setVehicleInput(Vehicle* vehicle, VehicleControls* input);
	VehicleState* getVehicleState(Vehicle* vehicle);
	VehicleProps* getVehicleProps(Vehicle* vehicle);
	VehicleConfig* getVehicleConfig(Vehicle* vehicle);
	void updateVehicleConfig(Vehicle* vehicle);
	void update(Vehicle* vehicle, float delta);
	
	Graph* createGraph();
	void deleteGraph(Graph* graph);
	void loadLinearGraph(Graph* graph, Vector2* refs, size_t refsCount);
	void loadBezierGraph(Graph* graph, Vector2* refs, size_t refsCount, size_t samplesPerSegment);
	Vector2* getGraphPoints(Graph* graph, size_t* pointsCount);
	float getGraphY(Graph* graph, float x);
	
	InputLogger* createInputLogger(Vehicle* vehicle);
	void logInput(InputLogger* inputLogger, float delta);
	void saveInputLogger(InputLogger* inputLogger, const char* filename);

#ifdef __cplusplus
}
#endif