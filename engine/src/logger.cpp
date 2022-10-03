#include <cstdio>
#include <cstdarg>
#include <cstring>

typedef void (*PrintFunc)(const char* toPrint);
PrintFunc printFunc = [](const char* toPrint) {printf("- %s\n", toPrint);};

void printDreg(const char* msgType, size_t maxSize, const char* msg) {
	char aux[maxSize + 15];
	snprintf(aux, maxSize + 15, "%s: %s", msgType, msg);
	
	printFunc(aux);
}

void printDregFormatted(const char* msgType, size_t maxSize, const char* msg, ...) {
	char aux[maxSize];
	
	va_list args;
	va_start(args, msg);
	vsnprintf(aux, maxSize, msg, args);
	va_end(args);
	
	printDreg(msgType, maxSize, aux);
}