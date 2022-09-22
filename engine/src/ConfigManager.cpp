#include "ConfigManager.h"

#include "dreg.h"
#include "Vehicle.h"
#include "Graph.h"

#include <cstdio>

ConfigManager::ConfigManager(bool createObjects) : isObjectsOwner(createObjects) {
	printFunc("Creating ConfigManager");
	
	if (isObjectsOwner) {
		config.power.throttleCurve = new Graph();
		config.power.engineCurve = new Graph();
		config.power.looseEngineRpmCurve = new Graph();
		config.power.engineBrakeCurve = new Graph();
		config.power.clutchCurve = new Graph();
		config.brakeCurve = new Graph();
		
		config.power.gearRatios = new std::vector<float>();
	}
	else {
		config.power.throttleCurve = NULL;
		config.power.engineCurve = NULL;
		config.power.looseEngineRpmCurve = NULL;
		config.power.engineBrakeCurve = NULL;
		config.power.clutchCurve = NULL;
		config.brakeCurve = NULL;
		
		config.power.gearRatios = NULL;
	}
}

ConfigManager::ConfigManager(const ConfigManager& original, bool fullClone) :
	config(original.config), isObjectsOwner(fullClone) {
	
	printFunc("Cloning ConfigManager");
	if (fullClone) {
		config.power.throttleCurve = new Graph(*config.power.throttleCurve);
		config.power.engineCurve = new Graph(*config.power.engineCurve);
		config.power.looseEngineRpmCurve = new Graph(*config.power.looseEngineRpmCurve);
		config.power.engineBrakeCurve = new Graph(*config.power.engineBrakeCurve);
		config.power.clutchCurve = new Graph(*config.power.clutchCurve);
		config.brakeCurve = new Graph(*config.brakeCurve);
		
		config.power.gearRatios = new std::vector<float>(*config.power.gearRatios);
	}
}

ConfigManager::~ConfigManager() {
	char aux[50];
	snprintf(aux, 50, "Destroying ConfigManager with %zu vehicles", vehicles.size());
	printFunc(aux);
	
	for (Vehicle* vehicle : vehicles)
		delete vehicle;
	
	if (isObjectsOwner) {
		delete config.power.throttleCurve;
		delete config.power.engineCurve;
		delete config.power.looseEngineRpmCurve;
		delete config.power.engineBrakeCurve;
		delete config.power.clutchCurve;
		delete config.brakeCurve;
		
		delete config.power.gearRatios;
	}
}

void ConfigManager::addVehicle(Vehicle* vehicle) {
	vehicles.insert(vehicle);
	vehicle->updateConfig();
}

void ConfigManager::removeVehicle(Vehicle* vehicle) {
	vehicles.erase(vehicle);
}

void ConfigManager::updateVehiclesConfig() const {
	for (Vehicle* vehicle : vehicles)
		vehicle->updateConfig();
}

void ConfigManager::loadDefaultConfig() {
	config.frontShaft = {0, 0, 0};
	config.rearShaft = {1, 0, 0};
	config.maxSteeringAngle = 20;
	config.mass = 500;
	
	config.power.gearRatios->reserve(8);
	config.power.gearRatios->push_back(-1);
	config.power.gearRatios->push_back(0);
	config.power.gearRatios->push_back(1);
	
	config.power.torqueToRpmAccel = 1;
	config.power.driveRatio = 1;
	config.wheels = {0.5, 0.3, 0.7, 0.56};
	
	//Load default graphs
	Vector2 refs[] = {{0, 0}, {1, 1}};
	config.power.throttleCurve->loadLinear(refs, 2);
	config.power.engineCurve->loadLinear(refs, 2);
	config.brakeCurve->loadLinear(refs, 2);
	
	refs[1].y = 0;
	config.power.looseEngineRpmCurve->loadLinear(refs, 2);
	config.power.engineBrakeCurve->loadLinear(refs, 2);
	
	refs[0] = {0, 1};
	refs[1] = {1, 0};
	config.power.clutchCurve->loadLinear(refs, 2);
}