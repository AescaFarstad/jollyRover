#include <VariableProto.h>

VariableProto::VariableProto()
{
}

VariableProto::~VariableProto()
{
}

void from_json(const json &j, ForceProto &force)
{
	auto spawn = j.at("spawn");
	force.id = j.at("id");
	force.spawnAA.x = spawn.at("x1");
	force.spawnAA.y = spawn.at("y1");
	force.spawnBB.x = spawn.at("x2");
	force.spawnBB.y = spawn.at("y2");
	force.freq[0] = j.at("freq")[0];
	force.freq[1] = j.at("freq")[1];
}

void VariableProto::load(json &source)
{
	fieldWidth = source.at("fieldWidth");
	fieldHeight = source.at("fieldHeight");

	routeStepSize = source.at("routeStepSize");
	minRouteSteps = source.at("minRouteSteps");
	maxRouteSteps = source.at("maxRouteSteps");

	stepAngleWindow = source.at("stepAngleWindow");
	
	intensity = source.at("war").at("intensity");
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



