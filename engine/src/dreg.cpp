#include "dreg.h"

#include "Graph.h"
#include "ConfigManager.h"
#include "ConfigParser.h"
#include "InputLogger.h"

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


FloatList* createFloatList() {
	return new std::vector<float>();
}

const float* getFloatList(const FloatList* list, size_t* size) {
	*size = list->size();
	return list->data();
}

void setFloatList(FloatList* list, const float* values, size_t valuesCount) {
	list->clear();
	list->reserve(valuesCount);
	
	for (size_t i = 0; i < valuesCount; i++)
		list->push_back(values[i]);
}

void deleteFloatList(FloatList* list) {
	delete list;
}

void deleteCharArray(char* array) {
	delete[] array;
}


Vehicle* createVehicle() {
	return new Vehicle();
}

void deleteVehicle(Vehicle* vehicle) {
	vehicle->clearConfig();
	delete vehicle;
}

void resetVehicle(Vehicle* vehicle) {
	vehicle->reset();
}

void setVehicleInput(Vehicle* vehicle, const VehicleControls* input) {
	vehicle->controls = *input;
}

const VehicleState* getVehicleState(const Vehicle* vehicle) {
	return &vehicle->state;
}

const VehicleProps* getVehicleProps(const Vehicle* vehicle) {
	return &vehicle->props;
}

ConfigManager* getConfigManager(Vehicle* vehicle) {
	return vehicle->configManager;
}

void setVehicleConfig(Vehicle* vehicle, ConfigManager* configManager) {
	vehicle->setConfig(configManager);
}

void update(Vehicle* vehicle, float delta) {
	vehicle->update(delta);
}


ConfigManager* createConfigManager(char createObjects) {
	return new ConfigManager(createObjects);
}

void deleteConfigManager(ConfigManager* configManager) {
	delete configManager;
}

ConfigManager* cloneConfig(const ConfigManager* configManager, char fullClone) {
	return new ConfigManager(*configManager, fullClone);
}

VehicleConfig* getVehicleConfig(ConfigManager* configManager) {
	return &configManager->config;
}

void updateConfig(ConfigManager* configManager) {
	configManager->updateVehiclesConfig();
}

void loadDefaultConfig(ConfigManager* configManager) {
	configManager->loadDefaultConfig();
}

char loadSerializedConfig(VehicleConfig* config, const char* serializedConfig) {
	ConfigParser parser;
	return parser.loadSerializedConfig(config, serializedConfig);
}

char* serializeConfig(const VehicleConfig* config) {
	ConfigParser parser;
	return parser.serializeConfig(config);
}


void setGraphSaveInitData(char saveInitData) {
	Graph::saveInitDataEnabled = saveInitData;
}

void setDefaultBezierSamples(size_t samplesPerSegment) {
	Graph::defaultBezierSamples = samplesPerSegment;
}


Graph* createGraph() {
	return new Graph();
}

void deleteGraph(Graph* graph) {
	delete graph;
}

Graph* cloneGraph(const Graph* graph) {
	return new Graph(*graph);
}

void loadLinearGraph(Graph* graph, const Vector2* refs, size_t refsCount) {
	graph->loadLinear(refs, refsCount);
}

void loadBezierGraph(Graph* graph, const Vector2* refs, size_t refsCount, size_t samplesPerSegment) {
	graph->loadBezier(refs, refsCount,
		samplesPerSegment == 0 ? Graph::defaultBezierSamples : samplesPerSegment);
}

const Vector2* getGraphInitData(const Graph* graph, char* isBezier, size_t* pointsCount) {
	const GraphInitData* initData = graph->getInitData();
	
	if (initData != NULL) {
		*isBezier = initData->type == GraphType::BEZIER;
		*pointsCount = initData->refs.size();
		return initData->refs.data();
	}
	else
		return NULL;
}

const Vector2* getGraphPoints(const Graph* graph, size_t* pointsCount) {
	return graph->getPoints(pointsCount);
}

float getGraphY(const Graph* graph, float x) {
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