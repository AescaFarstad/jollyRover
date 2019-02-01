#include <VariableProto.h>

VariableProto::VariableProto()
{
}

VariableProto::~VariableProto()
{
}

void VariableProto::load(json &source)
{
	fieldWidth = source.at("fieldWidth");
	fieldHeight = source.at("fieldHeight");

	routeStepSize = source.at("routeStepSize");
	minRouteSteps = source.at("minRouteSteps");
	maxRouteSteps = source.at("maxRouteSteps");

	stepAngleWindow = source.at("stepAngleWindow");
}
