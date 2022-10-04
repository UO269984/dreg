#include "dreg.h"

#include "logger.h"
#include "Graph.h"
#include "ConfigManager.h"
#include "ConfigParser.h"
#include "InputLogger.h"

#include <fstream>

#ifdef DREG_DEBUG
#define CHECK_RANGE_I(VALUE, MIN, MAX, NAME) checkInRange<int>(VALUE, MIN, MAX, NAME, "%s (%d) is out of range [%d, %d]")
#define CHECK_RANGE_F(VALUE, MIN, MAX, NAME) checkInRange<float>(VALUE, MIN, MAX, NAME, "%s (%f) is out of range [%f, %f]")
#define CHECK_POSITIVE_F(VALUE, NAME) checkPositive<float>(VALUE, NAME, "%s (%f) is negative")
#define CHECK_NOT_NULL(PTR, NAME) checkNotNull(PTR, NAME)
#define CHECK_CONFIG(CONFIG_MANAGER) checkConfig(CONFIG_MANAGER)

template<typename T>
static void checkInRange(T value, T min, T max, const char* name, const char* msg) {
	if (value < min || value > max) {
		DREG_WARN_FORMAT(90, msg, name, value, min, max);
	}
}

template<typename T>
static void checkPositive(T value, const char* name, const char* msg) {
	if (value < 0) {
		DREG_WARN_FORMAT(90, msg, name, value);
	}
}

static void checkNotNull(void* ptr, const char* name) {
	if (ptr == NULL) {
		DREG_ERROR_FORMAT(90, "%s is NULL", name);
	}
}

static void checkConfig(const ConfigManager* configManager) {
	const VehicleConfig* config = &configManager->config;
	
	CHECK_RANGE_F(config->maxSteeringAngle, 0, 180, "maxSteeringAngle");
	CHECK_POSITIVE_F(config->mass, "mass");
	CHECK_POSITIVE_F(config->wheels.diameter, "wheel diameter");
	CHECK_POSITIVE_F(config->wheels.brakeDiameter, "brake diameter");
	CHECK_POSITIVE_F(config->wheels.brakeStaticFrictionCoef, "brake static friction coef");
	CHECK_POSITIVE_F(config->wheels.brakeKineticFrictionCoef, "brake kinetic friction coef");
	checkNotNull(config->brakeCurve, "brake curve");
	checkNotNull(config->power.throttleCurve, "throttle curve");
	checkNotNull(config->power.engineCurve, "engine curve");
	checkNotNull(config->power.looseEngineRpmCurve, "loose engine rpm curve");
	checkNotNull(config->power.engineBrakeCurve, "engine brakeCcurve");
	checkNotNull(config->power.clutchCurve, "clutch curve");
	checkNotNull(config->power.gearRatios, "gear ratios");
}
#else
#define CHECK_RANGE_I(VALUE, MIN, MAX, NAME)
#define CHECK_RANGE_F(VALUE, MIN, MAX, NAME)
#define CHECK_POSITIVE_F(VALUE, NAME)
#define CHECK_NOT_NULL(PTR, NAME)
#define CHECK_CONFIG(CONFIG_MANAGER)
#endif

SaveFileFunc saveFileFunc = [](const char* filename, const char* data) {
	std::ofstream f;
	f.open(filename);
	f << data;
	f.close();
};

extern PrintFunc printFunc;
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
	delete vehicle;
}

void resetVehicle(Vehicle* vehicle) {
	vehicle->reset();
}

void setVehicleInput(Vehicle* vehicle, const VehicleControls* input) {
	CHECK_RANGE_F(input->throttle, 0, 1, "throttle");
	CHECK_RANGE_F(input->brake, 0, 1, "brake");
	CHECK_RANGE_F(input->steeringWheel, -1, 1, "steeringWheel");
	CHECK_RANGE_F(input->clutch, 0, 1, "clutch");
	CHECK_RANGE_I(input->gear, 0, vehicle->configManager->config.power.gearRatios->size() - 1, "gear");
	
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
	#ifdef DREG_DEBUG
	if (configManager != NULL)
		CHECK_CONFIG(configManager);
	#endif
	
	vehicle->setConfig(configManager);
}

void update(Vehicle* vehicle, float delta) {
	CHECK_NOT_NULL(vehicle->configManager, "vehicle configManager");
	CHECK_POSITIVE_F(delta, "update vehicle delta");
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
	CHECK_CONFIG(configManager);
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

char loadLinearGraph(Graph* graph, const Vector2* refs, size_t refsCount) {
	return graph->loadLinear(refs, refsCount);
}

char loadBezierGraph(Graph* graph, const Vector2* refs, size_t refsCount, size_t samplesPerSegment) {
	return graph->loadBezier(refs, refsCount,
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
	#ifdef DREG_DEBUG
	size_t pointsCount;
	graph->getPoints(&pointsCount);
	
	if (pointsCount == 0)
		DREG_ERROR("Trying to get a value on a not loaded graph");
	#endif
	
	return graph->getY(x);
}


InputLogger* createInputLogger(Vehicle* vehicle) {
	return new InputLogger(vehicle);
}

void logInput(InputLogger* inputLogger, float delta) {
	CHECK_POSITIVE_F(delta, "inputLogger delta");
	inputLogger->log(delta);
}

void saveInputLogger(InputLogger* inputLogger, const char* filename) {
	inputLogger->save(filename);
	delete inputLogger;
}