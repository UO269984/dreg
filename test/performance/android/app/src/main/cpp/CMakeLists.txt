cmake_minimum_required(VERSION 3.18.1)

project("testApi")

add_library( # Sets the name of the library.
	testApi
	SHARED

	testApi.cpp
	testPerformance.cpp)

add_library(
	dreg
	SHARED
	IMPORTED)

set_target_properties(
	dreg
	PROPERTIES IMPORTED_LOCATION
	${PROJECT_SOURCE_DIR}/dregLibs/${ANDROID_ABI}/libdreg.so)

include_directories(include)

target_link_libraries(
	testApi
	dreg)