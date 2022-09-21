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
	
	FloatList* createFloatList();
	const float* getFloatList(const FloatList* list, size_t* size);
	void setFloatList(FloatList* list, const float* values, size_t valuesCount);
	void deleteFloatList(FloatList* list);
	void deleteCharArray(char* array);
	
	Vehicle* createVehicle();
	void deleteVehicle(Vehicle* vehicle);
	void resetVehicle(Vehicle* vehicle);
	
	void setVehicleInput(Vehicle* vehicle, const VehicleControls* input);
	const VehicleState* getVehicleState(const Vehicle* vehicle);
	const VehicleProps* getVehicleProps(const Vehicle* vehicle);
	ConfigManager* getConfigManager(Vehicle* vehicle);
	void setVehicleConfig(Vehicle* vehicle, ConfigManager* configManager);
	void update(Vehicle* vehicle, float delta);
	
	ConfigManager* createConfigManager(char createObjects);
	void deleteConfigManager(ConfigManager* configManager);
	ConfigManager* cloneConfig(const ConfigManager* configManager, char fullClone);
	VehicleConfig* getVehicleConfig(ConfigManager* configManager);
	void updateConfig(ConfigManager* configManager);
	void loadDefaultConfig(ConfigManager* configManager);
	char loadSerializedConfig(VehicleConfig* config, const char* serializedConfig);
	char* serializeConfig(const VehicleConfig* config);
	
	void setGraphSaveInitData(char saveInitData);
	void setDefaultBezierSamples(size_t samplesPerSegment);
	
	Graph* createGraph();
	void deleteGraph(Graph* graph);
	Graph* cloneGraph(const Graph* graph);
	char loadLinearGraph(Graph* graph, const Vector2* refs, size_t refsCount);
	char loadBezierGraph(Graph* graph, const Vector2* refs, size_t refsCount, size_t samplesPerSegment);
	const Vector2* getGraphInitData(const Graph* graph, char* isBezier, size_t* pointsCount);
	const Vector2* getGraphPoints(const Graph* graph, size_t* pointsCount);
	float getGraphY(const Graph* graph, float x);
	
	InputLogger* createInputLogger(Vehicle* vehicle);
	void logInput(InputLogger* inputLogger, float delta);
	void saveInputLogger(InputLogger* inputLogger, const char* filename);

#ifdef __cplusplus
}
#endif