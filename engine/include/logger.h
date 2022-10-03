#pragma once

#include <cstring>

void printDreg(const char* msgType, size_t maxSize, const char* msg);
void printDregFormatted(const char* msgType, size_t maxSize, const char* msg, ...);

#ifndef LOG_LEVEL
	#ifdef DREG_DEBUG
		#define LOG_LEVEL 3
	#else
		#define LOG_LEVEL 1
	#endif
#endif

#if LOG_LEVEL > 2
#define DREG_LOG(MSG) printDreg("LOG", strlen(MSG), MSG)
#define DREG_LOG_FORMAT(SIZE, MSG, ...) printDregFormatted("LOG", SIZE, MSG, __VA_ARGS__)
#else
#define DREG_LOG(MSG)
#define DREG_LOG_FORMAT(SIZE, MSG, ...)
#endif

#if LOG_LEVEL > 1
#define DREG_WARN(MSG) printDreg("WARNING", strlen(MSG), MSG)
#define DREG_WARN_FORMAT(SIZE, MSG, ...) printDregFormatted("WARNING", SIZE, MSG, __VA_ARGS__)
#else
#define DREG_WARN(MSG)
#define DREG_WARN_FORMAT(SIZE, MSG, ...)
#endif

#if LOG_LEVEL > 0
#define DREG_ERROR(MSG) printDreg("ERROR", strlen(MSG), MSG)
#define DREG_ERROR_FORMAT(SIZE, MSG, ...) printDregFormatted("ERROR", SIZE, MSG, __VA_ARGS__)
#else
#define DREG_ERROR(MSG)
#define DREG_ERROR_FORMAT(SIZE, MSG, ...)
#endif