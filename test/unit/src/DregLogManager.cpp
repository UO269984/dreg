#include "DregLogManager.h"

DregLogManager dregLogManager;

void DregLogManager::newLog(const char* msg) {
	lastLog.clear();
	lastLog.append(msg);
	
	if (printEnabled)
		printf("%s\n", msg);
	
	size_t colonPos = lastLog.find(':');
	if (colonPos != lastLog.npos) {
		lastLog[colonPos] = 0;
		lastLogContent = lastLog.c_str() + colonPos + 2;
	}
	else
		lastLogContent = NULL;
}

void DregLogManager::clearLog() {
	lastLog.clear();
}

void DregLogManager::freeMemory() {
	lastLog.clear();
	lastLog.shrink_to_fit();
}

const char* DregLogManager::getLastLogLevel() const {
	return lastLog.c_str();
}

const char* DregLogManager::getLastLogContent() const {
	return lastLogContent;
}