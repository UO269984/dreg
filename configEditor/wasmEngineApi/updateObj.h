#pragma once

#include "drivingEngineApi.h"

void updateVehicleStateObj(PTR statePtr, emscripten::val obj);
void updateVehiclePropsObj(PTR propsPtr, emscripten::val obj);

void updateVehicleConfigStruct(PTR configPtr, emscripten::val obj);
void updateVehicleConfigObj(PTR configPtr, emscripten::val obj);