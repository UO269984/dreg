#include "TestVehicle.h"
#include "DregLogManager.h"

#include "dreg/ConfigManager.h"
#include "dreg/dreg.h"

#include <memory>

void TestVehicle::testStart() {
	configManager = new ConfigManager(true);
	configManager->loadDefaultConfig();
}

void TestVehicle::testEnd() {
	delete configManager;
}

typedef std::unique_ptr<Vehicle, std::function<void(Vehicle*)>> VehicleUPtr;

void TestVehicle::dregApi() {
	VehicleUPtr dregApiVehicleUPtr(createVehicle(), deleteVehicle);
	Vehicle* dregApiVehicle = dregApiVehicleUPtr.get();
	
	setVehicleConfig(dregApiVehicle, configManager);
	CuAssertPtrEquals(configManager, getConfigManager(dregApiVehicle));
	
	VehicleControls controls = {0, 0, 0, 0, 0};
	CHECK_LOG("", setVehicleInput(dregApiVehicle, &controls));
	
	controls.throttle = -0.2;
	CHECK_LOG("WARNING", setVehicleInput(dregApiVehicle, &controls))
	
	controls.throttle = 1.2;
	CHECK_LOG("WARNING", setVehicleInput(dregApiVehicle, &controls))
	
	controls.throttle = 0.1;
	controls.brake = 1.1;
	CHECK_LOG("WARNING", setVehicleInput(dregApiVehicle, &controls))
	
	controls.brake = 0.1;
	controls.steeringWheel = 1.1;
	CHECK_LOG("WARNING", setVehicleInput(dregApiVehicle, &controls))
	
	controls.steeringWheel = -0.1;
	controls.clutch = 1.1;
	CHECK_LOG("WARNING", setVehicleInput(dregApiVehicle, &controls))
	
	controls.clutch = 0.1;
	controls.gear = configManager->config.power.gearRatios->size();
	CHECK_LOG("WARNING", setVehicleInput(dregApiVehicle, &controls))
	
	controls.gear = -1;
	CHECK_LOG("WARNING", setVehicleInput(dregApiVehicle, &controls))
	CHECK_LOG("WARNING", update(dregApiVehicle, -0.1))
}