#pragma once
#include <json.hpp>
#include <Point.h>
using json = nlohmann::json;


class VariableProto
{
public:
	VariableProto();
	~VariableProto();

	int32_t fixedStepDuration;
	
	int32_t fieldWidth;
	int32_t fieldHeight;
	
	Point fieldCenter;

	int32_t routeStepSize;
	int32_t minRouteSteps;
	int32_t maxRouteSteps;
	float stepAngleWindow;
	
	float creepRestitution;
	int32_t additionalSpacing;	
	int32_t intensity;
	
	int32_t maxCreepSize;
	int32_t maxEventAnimationTime;
	

	void load(json &source);

private:

};
