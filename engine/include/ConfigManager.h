#pragma once

#include "VehicleApi.h"

#include <set>

class ConfigManager {
public:
	ConfigManager(bool createObjects);
	ConfigManager(const ConfigManager& original) : ConfigManager(original, true) {}
	ConfigManager(const ConfigManager& original, bool fullClone);
	~ConfigManager();
	
	void addVehicle(Vehicle* vehicle);
	void removeVehicle(Vehicle* vehicle);
	void updateVehiclesConfig() const;
	
	void loadDefaultConfig();
	
	VehicleConfig config;
	
private:
	bool isObjectsOwner;
	std::set<Vehicle*> vehicles;
};