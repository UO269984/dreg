#pragma once

#include "VehicleApi.h"

#include <memory>
#include <string_view>
#include <map>

enum class PropType {
	_FLOAT=0, FLOAT_LIST, VECTOR3, GRAPH
};

struct ConfigPropData {
	const char* name;
	long offset;
	PropType type;
};

class ConfigParser {
public:
	bool loadSerializedConfig(VehicleConfig* config, const char* serializedConfig);
	char* serializeConfig(const VehicleConfig* config);
	
	static void initConfigProps();
	static void clearConfigProps();

private:
	void setAttrib(VehicleConfig* config, const ConfigPropData& prop, std::string_view value);
	void parseFloatList(FloatList* arrayPtr, std::string_view value);
	void parseGraph(Graph* graph, std::string_view value);
	void serializeProp(const VehicleConfig* config, const ConfigPropData& prop, std::string& str);
	
	void parsingError(const char* msg);
	
	uint32_t lineNum;
	bool parsingSuccess;
	
	static std::unique_ptr<ConfigPropData[]> configProps;
	static std::map<std::string_view, const ConfigPropData*> configPropsMap;
};