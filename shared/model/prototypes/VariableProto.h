#pragma once
#include <json.hpp>
#include <Point.h>
using json = nlohmann::json;


class VariableProto
{
public:
	VariableProto();
	~VariableProto();

	int32_t fieldWidth;
	int32_t fieldHeight;
	
	Point fieldCenter;

	int32_t routeStepSize;
	int32_t minRouteSteps;
	int32_t maxRouteSteps;
	float stepAngleWindow;
	
	int32_t intensity;
	

	void load(json &source);

private:

};
