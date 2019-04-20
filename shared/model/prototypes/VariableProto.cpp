#include <VariableProto.h>

VariableProto::VariableProto()
{
}

VariableProto::~VariableProto()
{
}

void VariableProto::load(json &source)
{
	fixedStepDuration = source.at("fixedStepDuration");
	
	fieldWidth = source.at("fieldWidth");
	fieldHeight = source.at("fieldHeight");

	routeStepSize = source.at("routeStepSize");
	minRouteSteps = source.at("minRouteSteps");
	maxRouteSteps = source.at("maxRouteSteps");

	stepAngleWindow = source.at("stepAngleWindow");
	
	maxEventAnimationTime = source.at("maxEventAnimationTime");
	
	intensity = source.at("war").at("intensity");
	creepRestitution = source.at("war").at("creepRestitution");
	formationNumConnections = source.at("war").at("formationNumConnections");
	additionalSpacing = source.at("war").at("additionalSpacing");
	
	fieldCenter.x = fieldWidth/2;
	fieldCenter.y = fieldHeight/2;
	/*
	json forceArray = source.at("war").at("forces");
	
	for (auto i = forceArray.begin(); i != forceArray.end(); i++)
	{
		auto spawn = i->at("spawn");
		forces.emplace_back(ForceProto{
			i->at("id"), 
			Point(
				(float)spawn.at("x1"), 
				(float)spawn.at("y1")
				),
			Point(
				(float)spawn.at("x2"),
				(float)spawn.at("y2")
				),
			{i->at("freq")[0], i->at("freq")[1]}
		});
	}
	auto o = forces[0];
	auto o1 = forces[1];*/
}



