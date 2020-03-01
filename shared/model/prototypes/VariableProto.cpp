#include <VariableProto.h>

void VariableProto::load(nlohmann::json& source)
{
	fixedStepDuration = source.at("game").at("fixedStepDuration");
	
	reconnectWindow = source.at("reconnectWindow");
	maxLogicUpdatesPerFrame = source.at("maxLogicUpdatesPerFrame");
	heartbeatInterval = source.at("heartbeatInterval");
	heartbeatTimeout = source.at("heartbeatTimeout");
	
	fieldWidth = source.at("fieldWidth");
	fieldHeight = source.at("fieldHeight");

	routeStepSize = source.at("game").at("routeStepSize");
	minRouteSteps = source.at("game").at("minRouteSteps");
	maxRouteSteps = source.at("game").at("maxRouteSteps");

	stepAngleWindow = source.at("game").at("stepAngleWindow");
	
	maxEventAnimationTime = source.at("maxEventAnimationTime");
	
	intensity = source.at("game").at("war").at("intensity");
	creepRestitution = source.at("game").at("war").at("creepRestitution");
	additionalSpacing = source.at("game").at("war").at("additionalSpacing");
	baseAgroLevel = source.at("game").at("war").at("baseAgroLevel");
	
	fieldCenter.x = fieldWidth/2;
	fieldCenter.y = fieldHeight/2;
	
	repairSpeed = source.at("game").at("repairSpeed");
	refuelSpeed = source.at("game").at("refuelSpeed");
	repairRefuelBase = source.at("game").at("repairRefuelBase");
	
	carAgroPerUnit = source.at("game").at("war").at("carAgroPerUnit");
	carAgroThresholdPerSlot = source.at("game").at("war").at("carAgroThresholdPerSlot");
	carChaseBreakoutDistance = source.at("game").at("war").at("carChaseBreakoutDistance");
	
	carAgroGainPerUnit = source.at("game").at("carAgroGainPerUnit");
	carAgroFadeGainPerSecond = source.at("game").at("carAgroFadeGainPerSecond");
	
	formationMaxBravery = source.at("game").at("war").at("formationMaxBravery");
	formationMinBravery = source.at("game").at("war").at("formationMinBravery");
	retreatSpeedModifier = source.at("game").at("war").at("retreatSpeedModifier");
	minCreepsToRetreat = source.at("game").at("war").at("minCreepsToRetreat");
}



