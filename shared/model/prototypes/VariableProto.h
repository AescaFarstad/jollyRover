#pragma once
#include <json.hpp>
using json = nlohmann::json;

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

	void load(json &source);

private:

};
