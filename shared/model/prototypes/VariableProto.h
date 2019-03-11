#pragma once
#include <json.hpp>
#include <Edge.h>
using json = nlohmann::json;

class ForceProto
{
public:
	ForceProto() = default;
	~ForceProto() = default;
	
	
	int16_t id;
	Point spawnAA;
	Point spawnBB;
	int32_t freq[2];

};

void from_json(const json &j, ForceProto &force);

class VariableProto
{
public:
	VariableProto();
	~VariableProto();

	int32_t fieldWidth;
	int32_t fieldHeight;

	int32_t routeStepSize;
	int32_t minRouteSteps;
	int32_t maxRouteSteps;
	float stepAngleWindow;
	
	int32_t intensity;
	

	void load(json &source);

private:

};
