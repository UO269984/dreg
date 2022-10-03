#include "InputLogger.h"

#include "dreg.h"
#include "logger.h"

#include <string>

InputLogger::InputLogger(Vehicle* vehicle) : vehicle(vehicle) {
	DREG_LOG("Starting input logging");
}

void InputLogger::log(float delta) {
	if (inputs.size() != 0)
		time += delta;
	
	if (inputs.size() == 0 || ! sameInput(inputs.back(), vehicle->controls)) {
		times.push_back(time);
		inputs.push_back(vehicle->controls);
	}
}

void InputLogger::save(const char* filename) {
	std::string str("Time;Throttle;Brake;SteeringWheel;Clutch;Gear\n");
	str.reserve((inputs.size() + 2) * 47);
	char aux[55];
	
	std::list<VehicleControls>::iterator inputsIt = inputs.begin();
	std::list<float>::iterator timesIt = times.begin();
	
	while (inputsIt != inputs.end()) {
		snprintf(aux, 55, "%f;%f;%f;%f;%f;%d\n",
			*timesIt,
			inputsIt->throttle,
			inputsIt->brake,
			inputsIt->steeringWheel,
			inputsIt->clutch,
			inputsIt->gear);
		
		str.append(aux);
		timesIt++;
		inputsIt++;
	}
	saveFileFunc(filename, str.c_str());
	DREG_LOG_FORMAT(120, "Input logging saved: %s", filename);
}

bool InputLogger::sameInput(VehicleControls& c1, VehicleControls& c2) const {
	return c1.throttle == c2.throttle &&
		c1.brake == c2.brake &&
		c1.steeringWheel == c2.steeringWheel &&
		c1.clutch == c2.clutch &&
		c1.gear == c2.gear;
}