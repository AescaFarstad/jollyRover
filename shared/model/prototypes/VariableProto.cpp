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
	additionalSpacing = source.at("war").at("additionalSpacing");
	baseAgroLevel = source.at("war").at("baseAgroLevel");
	
	fieldCenter.x = fieldWidth/2;
	fieldCenter.y = fieldHeight/2;
	
	repairSpeed = source.at("repairSpeed");
	refuelSpeed = source.at("refuelSpeed");
	repairRefuelBase = source.at("repairRefuelBase");
	
	carAgroPerUnit = source.at("war").at("carAgroPerUnit");
	carAgroThresholdPerSlot = source.at("war").at("carAgroThresholdPerSlot");
	carChaseBreakoutDistance = source.at("war").at("carChaseBreakoutDistance");
	
	carAgroGainPerUnit = source.at("carAgroGainPerUnit");
	carAgroFadeGainPerSecond = source.at("carAgroFadeGainPerSecond");
	
	formationMaxBravery = source.at("war").at("formationMaxBravery");
	formationMinBravery = source.at("war").at("formationMinBravery");
	retreatSpeedModifier = source.at("war").at("retreatSpeedModifier");
	minCreepsToRetreat = source.at("war").at("minCreepsToRetreat");
}



