#include "TestConfigParser.h"
#include "DregLogManager.h"
#include "TestUtil.h"

#include "dreg/dreg.h"

#define TEST_CONFIG_STR "torqueToRpmAccel = 4.002000\n"\
	"driveRatio = 0.920000\n"\
	"wheelDiameter = 0.500000\n"\
	"brakeDiameter = 0.220000\n"\
	"brakeStaticFrictionCoef = 0.200000\n"\
	"brakeKineticFrictionCoef = 0.000000\n"\
	"maxSteeringAngle = 32.250000\n"\
	"mass = 125.000000\n"\
	"frontShaft = 1.000000,2.050000,3.000000\n"\
	"rearShaft = 3.000000,2.000000,1.000000\n"\
	"gearRatios = -5.223000,0.200000,0.000000,-1.000000,6.320000\n"\
	"throttleCurve = linear 0.360000 0.000000; 1.000000 1.000000\n"\
	"engineCurve = linear 5.000000 50.500000\n"\
	"looseEngineRpmCurve = linear 0.000000 0.000000; "\
		"0.500000 -1.500000; 0.600000 3.000000; 5.000000 5.000000\n"\
	"engineBrakeCurve = linear 1.000000 1.000000\n"\
	"clutchCurve = bezier 0.000000 0.000000; 0.200000 0.800000; "\
		"0.500000 0.500000; 1.000000 0.900000\n"\
	"brakeCurve = bezier 0.000000 0.000000; 0.300000 0.900000; "\
		"0.600000 0.500000; 1.000000 0.900000; 1.500000 1.500000; "\
		"2.000000 2.000000; 5.000000 5.000000"

void TestConfigParser::testStart() {
	setGraphSaveInitData(1);
	configParser = (ConfigParser*) malloc(sizeof(ConfigParser));
	
	configManager = new ConfigManager[2] {true, true};
	loadedConfigManager = configManager + 1;
	
	config = &configManager->config;
	loadedConfig = &loadedConfigManager->config;
	
	loadedConfig->frontShaft = {1, 2.05, 3};
	loadedConfig->rearShaft = {3, 2, 1};
	loadedConfig->maxSteeringAngle = 32.25;
	loadedConfig->mass = 125;
	
	loadedConfig->power.torqueToRpmAccel = 4.002;
	loadedConfig->power.driveRatio = 0.92;
	
	loadedConfig->wheels.diameter = 0.5;
	loadedConfig->wheels.brakeDiameter = 0.22;
	loadedConfig->wheels.brakeStaticFrictionCoef = 0.2;
	loadedConfig->wheels.brakeKineticFrictionCoef = 0;
	
	float gearRatios[] = {-5.223, 0.2, 0, -1, 6.32};
	setFloatList(loadedConfig->power.gearRatios, gearRatios, 5);
	
	const Vector2 refs1[] = {{0.36, 0}, {1, 1}};
	loadedConfig->power.throttleCurve->loadLinear(refs1, 2);
	const Vector2 refs2[] = {{5, 50.5}};
	loadedConfig->power.engineCurve->loadLinear(refs2, 1);
	const Vector2 refs3[] = {{0, 0}, {0.5, -1.5}, {0.6, 3}, {5, 5}};
	loadedConfig->power.looseEngineRpmCurve->loadLinear(refs3, 4);
	const Vector2 refs4[] = {{1, 1}};
	loadedConfig->power.engineBrakeCurve->loadLinear(refs4, 1);
	const Vector2 refs5[] = {{0, 0}, {0.2, 0.8}, {0.5, 0.5}, {1, 0.9}};
	loadedConfig->power.clutchCurve->loadBezier(refs5, 4);
	const Vector2 refs6[] = {{0, 0}, {0.3, 0.9}, {0.6, 0.5}, {1, 0.9}, {1.5, 1.5}, {2, 2}, {5, 5}};
	loadedConfig->brakeCurve->loadBezier(refs6, 7);
}

void TestConfigParser::testEnd() {
	setGraphSaveInitData(0);
	
	free(configParser);
	delete[] configManager;
}

void TestConfigParser::before() {
	configManager->loadDefaultConfig();
	new(configParser) ConfigParser();
}

void TestConfigParser::after() {
	configParser->~ConfigParser();
}

void TestConfigParser::invalidConfig() {
	CHECK_LOG("ERROR", CuAssertTrue(! configParser->loadSerializedConfig(config, "invalidProp = 44")))
	CHECK_LOG("ERROR", CuAssertTrue(! configParser->loadSerializedConfig(config, "mass = 44,43")))
	CHECK_LOG("ERROR", CuAssertTrue(! configParser->loadSerializedConfig(config, "mass = 44a3")))
	CHECK_LOG("ERROR", CuAssertTrue(! configParser->loadSerializedConfig(config, "frontShaft = 3,2")))
	CHECK_LOG("ERROR", CuAssertTrue(! configParser->loadSerializedConfig(config, "frontShaft = 3,2aa,4")))
	CHECK_LOG("ERROR", CuAssertTrue(! configParser->loadSerializedConfig(config, "gearRatios = 1 6")))
	CHECK_LOG("ERROR", CuAssertTrue(! configParser->loadSerializedConfig(config, "gearRatios = 1,6a")))
	
	CHECK_LOG("ERROR", CuAssertTrue(! configParser->loadSerializedConfig(config, "throttleCurve = invalidGraph 0 0; 1 1")))
	CHECK_LOG("ERROR", CuAssertTrue(! configParser->loadSerializedConfig(config, "throttleCurve = linear 0 4a0; 1 1")))
	CHECK_LOG("ERROR", CuAssertTrue(! configParser->loadSerializedConfig(config, "throttleCurve = linear 0 0 1; 1 1")))
	CHECK_LOG("ERROR", CuAssertTrue(! configParser->loadSerializedConfig(config, "throttleCurve = linear 0; 1 1")))
	CHECK_LOG("ERROR", CuAssertTrue(! configParser->loadSerializedConfig(config, "throttleCurve = linear ")))
	CHECK_LOG("ERROR", CuAssertTrue(! configParser->loadSerializedConfig(config, "throttleCurve = bezier 0 0; 1 1")))
}

void TestConfigParser::emptyConfig() {
	ConfigManager prevConfigManager(*configManager, true);
	CHECK_LOG("", CuAssertTrue(configParser->loadSerializedConfig(config, "")))
	TestUtil::checkSameConfigs(tc, config, &prevConfigManager.config, true);
}

void TestConfigParser::oneVariableConfig() {
	ConfigManager prevConfigManager(*configManager, true);
	
	CuAssertTrue(config->mass > 400);
	CHECK_LOG("", CuAssertTrue(configParser->loadSerializedConfig(config, "mass = 360.1")))
	CuAssertDblEquals(360.1, config->mass, 0.00001); //Check the mass has changed
	config->mass = prevConfigManager.config.mass;
	
	//Check other variables have not changed
	TestUtil::checkSameConfigs(tc, config, &prevConfigManager.config, true);
}

void TestConfigParser::completeConfig() {
	CHECK_LOG("", CuAssertTrue(configParser->loadSerializedConfig(config, TEST_CONFIG_STR)))
	TestUtil::checkSameConfigs(tc, config, loadedConfig, true);
}

typedef std::unique_ptr<char, std::function<void(char*)>> CharArrayUPtr;
typedef std::unique_ptr<ConfigManager, std::function<void(ConfigManager*)>> ConfigManagerUPtr;

void TestConfigParser::serializeConfigTest() {
	CharArrayUPtr serialized(configParser->serializeConfig(loadedConfig), deleteCharArray);
	CuAssertStrEquals(TEST_CONFIG_STR, serialized.get());
}

void TestConfigParser::dregApi() {
	CHECK_LOG("", CuAssertTrue(loadSerializedConfig(config, TEST_CONFIG_STR)))
	
	CharArrayUPtr serialized(serializeConfig(config), deleteCharArray);
	CuAssertStrEquals(TEST_CONFIG_STR, serialized.get());
	
	setGraphSaveInitData(0); //Init data not saved
	ConfigManagerUPtr configManagerUPtr(createConfigManager(1), deleteConfigManager);
	const VehicleConfig* dregApiConfig = getVehicleConfig(configManagerUPtr.get());
	
	CHECK_LOG("ERROR", serialized.reset(serializeConfig(dregApiConfig)))
}