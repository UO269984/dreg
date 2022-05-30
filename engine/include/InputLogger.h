#pragma once

#include "Vehicle.h"

#include <list>

class InputLogger {
public:
	InputLogger(Vehicle* vehicle);
	void log(float delta);
	void save(const char* filename);

private:
	bool sameInput(VehicleControls& c1, VehicleControls& c2) const;
	
	Vehicle* vehicle;
	
	float time = 0;
	std::list<float> times;
	std::list<VehicleControls> inputs;
};