#pragma once

#include <string>

#define CHECK_LOG(EXPECTED_TYPE, LINE) \
	dregLogManager.clearLog();\
	LINE;\
	CuAssertStrEquals(EXPECTED_TYPE, dregLogManager.getLastLogLevel());

class DregLogManager {
public:
	void newLog(const char* msg);
	void clearLog();
	void freeMemory();
	
	const char* getLastLogLevel() const;
	const char* getLastLogContent() const;
	
	bool printEnabled = true;

private:
	std::string lastLog;
	const char* lastLogContent;
};
extern DregLogManager dregLogManager;