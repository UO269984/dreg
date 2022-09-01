#include "ConfigParser.h"

#include "dreg.h"
#include "Graph.h"
#include "Util.h"

#include <cstring>
#include <string>

#define GET_STRUCT_ELEM(structPtr, offset, type) ((type*) (((uint8_t*) structPtr) + offset))
#define CREATE_CONFIG_PROP(structPtr, propPtr, type) (ConfigPropData) {((uint8_t*) propPtr) - (uint8_t*) &structPtr, type}

std::map<std::string_view, ConfigPropData> ConfigParser::configProps;
void ConfigParser::initConfigProps() {
	if (! configProps.empty())
		return;
	
	VehicleConfig config;
	
	configProps.emplace("torqueToRpmAccel", CREATE_CONFIG_PROP(config, &config.power.torqueToRpmAccel, PropType::_FLOAT));
	configProps.emplace("driveRatio", CREATE_CONFIG_PROP(config, &config.power.driveRatio, PropType::_FLOAT));
	configProps.emplace("wheelDiameter", CREATE_CONFIG_PROP(config, &config.wheels.diameter, PropType::_FLOAT));
	configProps.emplace("brakeDiameter", CREATE_CONFIG_PROP(config, &config.wheels.brakeDiameter, PropType::_FLOAT));
	configProps.emplace("brakeStaticFrictionCoef", CREATE_CONFIG_PROP(config, &config.wheels.brakeStaticFrictionCoef, PropType::_FLOAT));
	configProps.emplace("brakeKineticFrictionCoef", CREATE_CONFIG_PROP(config, &config.wheels.brakeKineticFrictionCoef, PropType::_FLOAT));
	configProps.emplace("maxSteeringAngle", CREATE_CONFIG_PROP(config, &config.maxSteeringAngle, PropType::_FLOAT));
	configProps.emplace("mass", CREATE_CONFIG_PROP(config, &config.mass, PropType::_FLOAT));
	
	configProps.emplace("gearRatios", CREATE_CONFIG_PROP(config, &config.power.gearRatios, PropType::FLOAT_LIST));
	
	configProps.emplace("frontShaft", CREATE_CONFIG_PROP(config, &config.frontShaft, PropType::VECTOR3));
	configProps.emplace("rearShaft", CREATE_CONFIG_PROP(config, &config.rearShaft, PropType::VECTOR3));
	
	configProps.emplace("throttleCurve", CREATE_CONFIG_PROP(config, &config.power.throttleCurve, PropType::GRAPH));
	configProps.emplace("engineCurve", CREATE_CONFIG_PROP(config, &config.power.engineCurve, PropType::GRAPH));
	configProps.emplace("looseEngineRpmCurve", CREATE_CONFIG_PROP(config, &config.power.looseEngineRpmCurve, PropType::GRAPH));
	configProps.emplace("engineBrakeCurve", CREATE_CONFIG_PROP(config, &config.power.engineBrakeCurve, PropType::GRAPH));
	configProps.emplace("clutchCurve", CREATE_CONFIG_PROP(config, &config.power.clutchCurve, PropType::GRAPH));
	configProps.emplace("brakeCurve", CREATE_CONFIG_PROP(config, &config.brakeCurve, PropType::GRAPH));
}

bool ConfigParser::loadSerializedConfig(VehicleConfig* config, const char* serializedConfig) {
	initConfigProps();
	lineNum = 1;
	parsingSuccess = true;
	
	std::list<std::string_view> lines;
	Util::split(serializedConfig, '\n', lines);
	
	for (const std::string_view& line : lines) {
		size_t equalPos = line.find('=');
		
		if (equalPos == line.npos)
			parsingError("'=' not found");
		
		std::string_view name = Util::removeBlankEnds(line.substr(0, equalPos));
		std::map<std::string_view, ConfigPropData>::iterator prop = configProps.find(name);
		
		if (prop != configProps.end())
			setAttrib(config, prop->second, Util::removeBlankEnds(line.substr(equalPos + 1)));
		
		else
			parsingError("Invalid property");
		
		lineNum++;
		if (! parsingSuccess)
			break;
	}
	return parsingSuccess;
}

void ConfigParser::setAttrib(VehicleConfig* config, ConfigPropData prop, std::string_view value) {
	switch (prop.type) {
		case PropType::_FLOAT: {
			float val;
			
			if (Util::strToFloat(value, &val))
				*GET_STRUCT_ELEM(config, prop.offset, float) = val;
			else
				parsingError("Error parsing float");
			break;
		}
		case PropType::FLOAT_LIST:
			parseFloatList(*GET_STRUCT_ELEM(config, prop.offset, FloatList*), value);
			break;
		
		case PropType::VECTOR3: {
			std::vector<float> values;
			parseFloatList(&values, value);
			
			if (values.size() == 3)
				*GET_STRUCT_ELEM(config, prop.offset, Vector3) = {values[0], values[1], values[2]};
			else
				parsingError("Invalid number of components in Vector3");
			break;
		}
		
		case PropType::GRAPH:
			parseGraph(*GET_STRUCT_ELEM(config, prop.offset, Graph*), value);
			break;
	}
}

void ConfigParser::parseFloatList(FloatList* list, std::string_view value) {
	std::list<std::string_view> valuesSplitted;
	Util::split(value, ',', valuesSplitted);
	
	float parsed;
	list->clear();
	list->reserve(valuesSplitted.size());
	
	for (const std::string_view& valueStr : valuesSplitted) {
		if (Util::strToFloat(Util::removeBlankEnds(valueStr), &parsed))
			list->push_back(parsed);
		
		else
			parsingError("Invalid float");
	}
}

void ConfigParser::parseGraph(Graph* graph, std::string_view value) {
	size_t spacePos = value.find(' ');
	std::string_view type = value.substr(0, spacePos);
	
	std::list<std::string_view> pointsStrList;
	Util::split(value.substr(spacePos + 1), ';', pointsStrList);
	
	Vector2 points[pointsStrList.size()];
	Vector2* curPoint = points;
	
	for (std::string_view& pointStr : pointsStrList) {
		pointStr = Util::removeBlankEnds(pointStr);
		spacePos = pointStr.find(' ');
		
		if (! Util::strToFloat(Util::removeBlankEnds(pointStr.substr(0, spacePos)), &curPoint->x) ||
			! Util::strToFloat(Util::removeBlankEnds(pointStr.substr(spacePos + 1)), &curPoint->y)) {
			
			parsingError("Invalid graph point");
		}
		curPoint++;
	}
	
	if (type.compare("linear") == 0)
		graph->loadLinear(points, pointsStrList.size());
	
	else if (type.compare("bezier") == 0)
		graph->loadBezier(points, pointsStrList.size(), Graph::defaultBezierSamples);
	
	else
		parsingError("Invalid graph type");
}

char* ConfigParser::serializeConfig(const VehicleConfig* config) {
	initConfigProps();
	
	std::string str;
	str.reserve(1000);
	
	for (const std::pair<std::string_view, ConfigPropData>& prop : configProps) {
		str.append(prop.first);
		str.append(" = ");
		serializeProp(config, prop.second, str);
		str.append(1, '\n');
	}
	char* retStr = new char[str.size() + 1];
	str.copy(retStr, str.size());
	retStr[str.size()] = 0;
	
	return retStr;
}

void ConfigParser::serializeProp(const VehicleConfig* config, ConfigPropData prop, std::string& str) {
	char aux[30];
	
	switch (prop.type) {
		case PropType::_FLOAT:
			snprintf(aux, 30, "%f", *GET_STRUCT_ELEM(config, prop.offset, float));
			str.append(aux);
			break;
		
		case PropType::FLOAT_LIST: {
			FloatList* list = *GET_STRUCT_ELEM(config, prop.offset, FloatList*);
			
			for (float elem : *list) {
				snprintf(aux, 30, "%f,", elem);
				str.append(aux);
			}
			break;
		}
		
		case PropType::VECTOR3: {
			Vector3* vector = GET_STRUCT_ELEM(config, prop.offset, Vector3);
			snprintf(aux, 30, "%f,%f,%f", vector->x, vector->y, vector->z);
			str.append(aux);
			break;
		}
		
		case PropType::GRAPH: {
			Graph* graph = *GET_STRUCT_ELEM(config, prop.offset, Graph*);
			const GraphInitData* initData = graph->getInitData();
			
			if (initData == NULL) {
				printFunc("Error serializing config, Graph init data not found");
				return;
			}
			str.append(initData->type == GraphType::LINEAR ? "linear " : "bezier ");
			
			for (const Vector2& point : initData->refs) {
				snprintf(aux, 30, "%f %f; ", point.x, point.y);
				str.append(aux);
			}
			str.erase(str.size() - 2); //Delete ; at the end
			break;
		}
	}
}

void ConfigParser::parsingError(const char* msg) {
	parsingSuccess = false;
	
	char errorMsg[150];
	snprintf(errorMsg, 150, "Error loading config, line %d, %s", lineNum, msg);
	printFunc(errorMsg);
}