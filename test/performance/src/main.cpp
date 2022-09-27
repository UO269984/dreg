#include "testPerformance.h"
#include "dreg/Util.h"

#include <cstdio>

void parseParams(int argc, const char** argv, TestsConfig* config) {
	for (int i = 1; i < argc; i++) { //Ignore the first param
		std::string_view param(argv[i]);
		size_t equalPos = param.find('=');
		int value;
		
		if (equalPos != param.npos && Util::strToInt(param.substr(equalPos + 1), &value)) {
			std::string_view name = param.substr(0, equalPos);
			
			if (name.compare("bezierSamples") == 0)
				config->bezierSamples = value;
			
			else if (name.compare("singleVehicleIters") == 0)
				config->singleVehicleIters = value;
			
			else if (name.compare("multipleVehiclesIters") == 0)
				config->multipleVehiclesIters = value;
			
			else if (name.compare("multipleVehiclesCount") == 0)
				config->multipleVehiclesCount = value;
			
			else if (name.compare("createVehiclesCount") == 0)
				config->createVehiclesCount = value;
			
			else if (name.compare("loadConfigIters") == 0)
				config->loadConfigIters = value;
			
			else
				printf("Invalid param: \"%s\"\n", argv[i]);
		}
		else
			printf("Error parsing param: \"%s\"\n", argv[i]);
	}
}

int main(int argc, const char** argv) {
	TestsConfig config;
	loadDefaultConfig(&config);
	parseParams(argc, argv, &config);

	if (! allTests(&config))
		printf("ERROR: Invalid config\n");
	
	deleteTestsConfig(&config);
	return 0;
}