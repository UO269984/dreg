#include "TestConfigManager.h"
#include "TestUtil.h"

#include "dreg/dreg.h"
#include "dreg/Vehicle.h"

#include <memory>

void TestConfigManager::testStart() {
	configManager = (ConfigManager*) malloc(sizeof(ConfigManager));
	config = &configManager->config;
}

void TestConfigManager::testEnd() {
	free(configManager);
}

void TestConfigManager::before() {
	new(configManager) ConfigManager(true);
	configManager->loadDefaultConfig();
}

void TestConfigManager::after() {
	configManager->~ConfigManager();
}

void TestConfigManager::constructor() {
	//ConfigManager with objects created
	checkPointersLoaded(config, true);
	
	configManager->~ConfigManager();
	new(configManager) ConfigManager(false); //ConfigManager without objects created
	checkPointersLoaded(config, false);
}

void TestConfigManager::checkGraph(const Graph* graph, bool expectedGraph) const {
	size_t pointsCount;
	
	CuAssertTrue((graph != NULL) == expectedGraph);
	if (expectedGraph)
		CuAssertPtrNotNull(graph->getPoints(&pointsCount));
}

void TestConfigManager::checkPointersLoaded(const VehicleConfig* config, bool expectedObjects) const {
	checkGraph(config->power.throttleCurve, expectedObjects);
	checkGraph(config->power.engineCurve, expectedObjects);
	checkGraph(config->power.looseEngineRpmCurve, expectedObjects);
	checkGraph(config->power.engineBrakeCurve, expectedObjects);
	checkGraph(config->power.clutchCurve, expectedObjects);
	checkGraph(config->brakeCurve, expectedObjects);
	
	CuAssertTrue((config->power.gearRatios != NULL) == expectedObjects);
}

void TestConfigManager::vehicles() {
	Vehicle vehicle1, vehicle2;
	CuAssertTrue(vehicle1.configManager == NULL);
	CuAssertTrue(vehicle2.configManager == NULL);
	configManager->updateVehiclesConfig();
	
	vehicle1.setConfig(configManager);
	vehicle2.setConfig(configManager);
	CuAssertPtrNotNull(vehicle1.configManager);
	CuAssertPtrNotNull(vehicle2.configManager);
	configManager->updateVehiclesConfig();
	
	vehicle1.clearConfig();
	CuAssertTrue(vehicle1.configManager == NULL);
	CuAssertPtrNotNull(vehicle2.configManager);
	configManager->updateVehiclesConfig();
	
	vehicle2.setConfig(NULL); //Equivalent to clearConfig
	CuAssertTrue(vehicle1.configManager == NULL);
	CuAssertTrue(vehicle2.configManager == NULL);
	configManager->updateVehiclesConfig();
}

void TestConfigManager::clone() {
	ConfigManager clone(*configManager, false);
	//Pointers to objects should point to the same object. Is not a full copy
	TestUtil::checkSameConfigs(tc, config, &clone.config, false);
	
	clone.~ConfigManager();
	new(&clone) ConfigManager(*configManager, true);
	TestUtil::checkSameConfigs(tc, config, &clone.config, true);
	
	//Objects should have been copied. Is a full copy
	CuAssertTrue(config->power.throttleCurve != clone.config.power.throttleCurve);
	CuAssertTrue(config->power.engineCurve != clone.config.power.engineCurve);
	CuAssertTrue(config->power.looseEngineRpmCurve != clone.config.power.looseEngineRpmCurve);
	CuAssertTrue(config->power.engineBrakeCurve != clone.config.power.engineBrakeCurve);
	CuAssertTrue(config->power.clutchCurve != clone.config.power.clutchCurve);
	CuAssertTrue(config->brakeCurve != clone.config.brakeCurve);
	CuAssertTrue(config->power.gearRatios != clone.config.power.gearRatios);
}

typedef std::unique_ptr<ConfigManager, std::function<void(ConfigManager*)>> ConfigManagerUPtr;

void TestConfigManager::dregApi() {
	Vehicle vehicle1;
	
	ConfigManagerUPtr dregApiConfigManagerUptr(createConfigManager(1), deleteConfigManager);
	ConfigManager* dregApiConfigManager = dregApiConfigManagerUptr.get();
	loadDefaultConfig(dregApiConfigManager);
	
	CuAssertTrue(getVehicleConfig(dregApiConfigManager) == &dregApiConfigManager->config);
	setVehicleConfig(&vehicle1, dregApiConfigManager);
	updateConfig(dregApiConfigManager);
	
	setVehicleConfig(&vehicle1, NULL);
	
	ConfigManagerUPtr clone(cloneConfig(dregApiConfigManager, 0), deleteConfigManager);
	TestUtil::checkSameConfigs(tc, &dregApiConfigManager->config, &clone->config, false);
}