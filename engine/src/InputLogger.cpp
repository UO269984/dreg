#include "InputLogger.h"

#include "dreg.h"

#include <string>

InputLogger::InputLogger(Vehicle* vehicle) : vehicle(vehicle) {
	printFunc("Starting input logging");
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
	str.reserve((inputs.size() + 2) * 36);
	
	std::list<VehicleControls>::iterator inputsIt = inputs.begin();
	std::list<float>::iterator timesIt = times.begin();
	
	while (inputsIt != inputs.end()) {
		str += std::to_string(*timesIt) + ";"+
			std::to_string(inputsIt->throttle) + ";" +
			std::to_string(inputsIt->brake) + ";" +
			std::to_string(inputsIt->steeringWheel) + ";" +
			std::to_string(inputsIt->clutch) + ";" +
			std::to_string(inputsIt->gear) + "\n";
		
		timesIt++;
		inputsIt++;
	}
	saveFileFunc(filename, str.c_str());
	printFunc(("Input logging saved: " + std::string(filename)).c_str());
}

bool InputLogger::sameInput(VehicleControls& c1, VehicleControls& c2) const {
	return c1.throttle == c2.throttle &&
		c1.brake == c2.brake &&
		c1.steeringWheel == c2.steeringWheel &&
		c1.clutch == c2.clutch &&
		c1.gear == c2.gear;
}