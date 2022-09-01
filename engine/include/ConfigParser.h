#pragma once

#include "VehicleApi.h"

#include <string_view>
#include <map>

enum class PropType {
	_FLOAT=0, FLOAT_LIST, VECTOR3, GRAPH
};

struct ConfigPropData {
	long offset;
	PropType type;
};

class ConfigParser {
public:	
	bool loadSerializedConfig(VehicleConfig* config, const char* serializedConfig);
	char* serializeConfig(const VehicleConfig* config);

private:
	void setAttrib(VehicleConfig* config, ConfigPropData prop, std::string_view value);
	void parseFloatList(FloatList* arrayPtr, std::string_view value);
	void parseGraph(Graph* graph, std::string_view value);
	void serializeProp(const VehicleConfig* config, ConfigPropData prop, std::string& str);
	
	void parsingError(const char* msg);
	
	uint32_t lineNum;
	bool parsingSuccess;
	
	static void initConfigProps();
	static std::map<std::string_view, ConfigPropData> configProps;
};