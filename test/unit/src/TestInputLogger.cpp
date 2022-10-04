#include "TestInputLogger.h"
#include "DregLogManager.h"

#include "dreg/dreg.h"

#define LINE1_INPUT_LOG "Time;Throttle;Brake;SteeringWheel;Clutch;Gear\n"
std::string TestInputLogger::lastFileSave;

void TestInputLogger::recordFileSave(const char* filename, const char* data) {
	lastFileSave.clear();
	lastFileSave.append(data);
}

void TestInputLogger::testStart() {
	setSaveFileFunc(recordFileSave);
	
	vehicle = new Vehicle();
	inputLogger = (InputLogger*) malloc(sizeof(InputLogger));
}

void TestInputLogger::testEnd() {
	lastFileSave.clear();
	lastFileSave.shrink_to_fit();
	
	delete vehicle;
	free(inputLogger);
}

void TestInputLogger::before() {
	new(inputLogger) InputLogger(vehicle);
}

void TestInputLogger::after() {
	inputLogger->~InputLogger();
}

void TestInputLogger::noRecords() {
	inputLogger->save("File1");
	CuAssertStrEquals(LINE1_INPUT_LOG, lastFileSave.c_str());
}

void TestInputLogger::oneRecord() {
	vehicle->controls = {0.8, 0, 0.3, 0, 2};
	inputLogger->log(0.2);
	inputLogger->save("File2");
	CuAssertStrEquals(LINE1_INPUT_LOG "0.000000;0.800000;0.000000;0.300000;0.000000;2\n", lastFileSave.c_str());
}

void TestInputLogger::multipleRecords() {
	vehicle->controls = {0.8, 0, 0.3, 0, 2};
	inputLogger->log(0.8);
	
	vehicle->controls = {0.8, 0, 0.3, 0, 3};
	inputLogger->log(0.1);
	vehicle->controls = {0.81, 0, 0.3, 0, 3};
	inputLogger->log(0.8);
	vehicle->controls = {0, 0.5, 0, 0, 2};
	inputLogger->log(0.5);
	vehicle->controls = {0, 0.5, 0, 0, 2}; //Same as the last, only adds delta
	inputLogger->log(0.2);
	
	vehicle->controls = {0.6, 0, 0, 0, 2};
	vehicle->controls = {1, 0, 0, 0, 2};
	inputLogger->log(0.2);
	
	vehicle->controls = {0, 0.5, 0, 0, 2}; //Same as the second to last
	inputLogger->log(0);
	
	inputLogger->save("File3");
	const char* expectedInputLog = LINE1_INPUT_LOG
		"0.000000;0.800000;0.000000;0.300000;0.000000;2\n"
		"0.100000;0.800000;0.000000;0.300000;0.000000;3\n"
		"0.900000;0.810000;0.000000;0.300000;0.000000;3\n"
		"1.400000;0.000000;0.500000;0.000000;0.000000;2\n"
		"1.800000;1.000000;0.000000;0.000000;0.000000;2\n"
		"1.800000;0.000000;0.500000;0.000000;0.000000;2\n";
	CuAssertStrEquals(expectedInputLog, lastFileSave.c_str());
}

void TestInputLogger::dregApi() {
	InputLogger* inputLogger2 = createInputLogger(vehicle);
	
	vehicle->controls = {0, 1, 0, 0, 2};
	logInput(inputLogger2, 1);
	CHECK_LOG("WARNING", logInput(inputLogger2, -0.1))
	
	saveInputLogger(inputLogger2, "File4");
	const char* expectedInputLog = LINE1_INPUT_LOG
		"0.000000;0.000000;1.000000;0.000000;0.000000;2\n";
	CuAssertStrEquals(expectedInputLog, lastFileSave.c_str());
}