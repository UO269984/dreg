#include "testPerformance.h"
#include "dreg/ConfigParser.h"

#include <chrono>
#include <cstdio>
#include <cstring>

PrintFunc testPrintFunc = [](const char* toPrint) {printf("%s", toPrint);};
void setTestPrintFunc(PrintFunc newPrintFunc) {
	testPrintFunc = newPrintFunc;
}

class Timer {
public:
	void start() {
		t0 = getCurTime();
	}
	void stop() {
		lastInterval = getCurTime() - t0;
		accumulatedTime += lastInterval;
	}
	
	//The time is in milliseconds
	double getLastInterval() const {
		return lastInterval / 1000.0;
	}
	double getTime() const {
		return accumulatedTime / 1000.0;
	}

private:
	long int getCurTime() const {
		return std::chrono::time_point_cast<std::chrono::microseconds>(
			std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	}
	long int t0;
	long int lastInterval = 0;
	long int accumulatedTime = 0;
};


bool updateVehicle(Timer& timer, size_t iters, size_t vehiclesCount, const char* serializedConfig) {
	Vehicle** vehicles = new Vehicle*[vehiclesCount];
	ConfigManager* config = createConfigManager(1);
	
	loadDefaultConfig(config);
	if (serializedConfig != NULL && ! loadSerializedConfig(getVehicleConfig(config), serializedConfig))
		return false;
	
	for (size_t i = 0; i < vehiclesCount; i++) {
		vehicles[i] = createVehicle();
		setVehicleConfig(vehicles[i], config);
	}
	
	VehicleControls controls = {1, 0.05, 0.3, 0, 2};
	bool throtleActicve = true;
	
	timer.start();
	for (size_t i = 0; i < iters; i++) {
		throtleActicve = ! throtleActicve;
		controls.throttle = throtleActicve ? 1 : 0;
		
		for (size_t iVehicle = 0; iVehicle < vehiclesCount; iVehicle++) {
			setVehicleInput(vehicles[iVehicle], &controls);
			update(vehicles[iVehicle], 0.002);
		}
	}
	timer.stop();
	
	deleteConfigManager(config);
	for (size_t i = 0; i < vehiclesCount; i++)
		deleteVehicle(vehicles[i]);
	
	delete[] vehicles;
	return true;
}

void createVehicles(Timer& timer, size_t vehiclesCount) {
	Vehicle** vehicles = new Vehicle*[vehiclesCount];
	ConfigManager* config = createConfigManager(1);
	loadDefaultConfig(config);
	
	timer.start();
	for (size_t i = 0; i < vehiclesCount; i++) {
		vehicles[i] = createVehicle();
		setVehicleConfig(vehicles[i], config);
	}
	timer.stop();
	
	deleteConfigManager(config);
	for (size_t i = 0; i < vehiclesCount; i++)
		deleteVehicle(vehicles[i]);
	
	delete[] vehicles;
}

bool loadConfig(Timer& timer, size_t iters, const char* serializedConfig) {
	ConfigManager* configManager = createConfigManager(1);
	VehicleConfig* config = getVehicleConfig(configManager);
	
	timer.start();
	for (size_t i = 0; i < iters; i++) {
		if (! loadSerializedConfig(config, serializedConfig)) {
			deleteConfigManager(configManager);
			return false;
		}
	}
	timer.stop();
	
	deleteConfigManager(configManager);
	return true;
}


static void printTestResult(const Timer& timer, size_t totalIters, const char* testName, const char* testConfig) {
	char aux[300];
	snprintf(aux, 300, "%s:\n\t%s\n\tTime: %lf s\n\tAverge time per iter: %lf \xC2\xB5s\n\n", testName, testConfig,
		timer.getLastInterval() / 1000, (timer.getLastInterval() / totalIters) * 1000);
	
	testPrintFunc(aux);
}

bool allTests(const TestsConfig* config) {
	Timer timer;
	char aux[150];
	setPrintFunc([](const char* toPrint) {}); //Disable dreg print
	setDefaultBezierSamples(config->bezierSamples);
	bool testsSuccess = true;
	
	testPrintFunc("Performance test\n");
	testsSuccess = testsSuccess && updateVehicle(timer, config->singleVehicleIters, 1, config->vehicleConfigVehicle);
	snprintf(aux, 150, "1 Vehicle, %zu Iterations", config->singleVehicleIters);
	printTestResult(timer, config->singleVehicleIters, "Update vehicle", aux);
	
	testsSuccess = testsSuccess && updateVehicle(timer,
		config->multipleVehiclesIters, config->multipleVehiclesCount, config->vehicleConfigVehicle);
	snprintf(aux, 150, "%zu Vehicles, %zu Iterations", config->multipleVehiclesCount, config->multipleVehiclesIters);
	printTestResult(timer, config->multipleVehiclesIters * config->multipleVehiclesCount, "Update vehicle", aux);
	
	createVehicles(timer, config->createVehiclesCount);
	snprintf(aux, 150, "%zu Vehicles", config->createVehiclesCount);
	printTestResult(timer, config->createVehiclesCount, "Create vehicles", aux);
	
	testsSuccess = testsSuccess && loadConfig(timer, config->loadConfigIters, config->vehicleConfigLoadConfig);
	snprintf(aux, 150, "%zu bezier graph samples, %zu load config iters", config->bezierSamples, config->loadConfigIters);
	printTestResult(timer, config->loadConfigIters, "Load config", aux);
	
	snprintf(aux, 150, "Total time in tests: %lfs\n", timer.getTime() / 1000);
	testPrintFunc(aux);
	ConfigParser::clearConfigProps();
	return testsSuccess;
}


char* cloneCharArray(const char* str, size_t size) {
	char* clone = new char[size + 1];
	memcpy(clone, str, (size + 1) * sizeof(char));
	return clone;
}

void deleteTestsConfig(TestsConfig* config) {
	if (config->vehicleConfigVehicle != NULL)
		delete[] config->vehicleConfigVehicle;
	
	if (config->vehicleConfigLoadConfig != NULL)
		delete[] config->vehicleConfigLoadConfig;
	
	config->vehicleConfigVehicle = NULL;
	config->vehicleConfigLoadConfig = NULL;
}

void loadDefaultConfig(TestsConfig* config) {	
	config->bezierSamples = 20;
	config->singleVehicleIters = 10000000;
	config->multipleVehiclesIters = 40000;
	config->multipleVehiclesCount = 500;
	config->createVehiclesCount = 20000;
	config->loadConfigIters = 20000;
	
	const char* defaultConfig = "brakeCurve = bezier 0.000000 0.000000; 0.367932 264.913361; 0.534878 1979.929810; 0.610540 3180.330811; 0.638365 3621.791016; 0.912682 8353.781250; 0.996079 9721.772461\n"
		"brakeDiameter = 0.600000\n"
		"brakeKineticFrictionCoef = 1.800000\n"
		"brakeStaticFrictionCoef = 2.500000\n"
		"clutchCurve = bezier -0.002778 292.886108; 0.000988 38.364029; -0.013383 25.619619; 0.087648 19.838934; 0.456917 -1.289404; 0.711984 0.113131; 1.000000 0.000000\n"
		"driveRatio = 8.000000\n"
		"engineBrakeCurve = linear 19.444445 13.888888; 1011.462463 40.002598\n"
		"engineCurve = bezier -81.369934 1.026431; 414.281830 21.089050; 1063.045166 45.771049; 1442.755859 58.160000; 1979.407837 75.669525; 2704.067627 88.425575; 3676.119385 88.456200; 4275.813965 88.475090; 4638.327148 72.029701; 4798.726562 62.864040; 5074.416504 47.110382; 5470.503906 20.332886; 5631.168457 -3.141718\n"
		"frontShaft = 0.000000,0.000000,0.000000\n"
		"gearRatios = -2.500000,0.000000,3.800000,2.400000,1.200000,0.600000\n"
		"looseEngineRpmCurve = linear 0.071324 2247.906006; 0.999287 6160.127930\n"
		"mass = 1000.000000\n"
		"maxSteeringAngle = 20.000000\n"
		"rearShaft = 1.000000,0.000000,0.000000\n"
		"throttleCurve = linear 0.000000 0.100000; 1.000000 1.000000\n"
		"torqueToRpmAccel = 80.000000\n"
		"wheelDiameter = 1.243000\n";
	
	deleteTestsConfig(config);
	size_t configSize = strlen(defaultConfig);
	config->vehicleConfigVehicle = cloneCharArray(defaultConfig, configSize);
	config->vehicleConfigLoadConfig = cloneCharArray(defaultConfig, configSize);
}