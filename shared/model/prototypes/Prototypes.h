#pragma once

#include <json.hpp>
#include <vector>
#include <VariableProto.h>
#include <Obstacle.h>
#include <CarProto.h>
#include <LootProto.h>
#include <CreepProto.h>
#include <FormationProto.h>
#include <ObjectiveProto.h>

using json = nlohmann::json;

class Prototypes
{
public:
	Prototypes();
	~Prototypes();

	VariableProto variables;
	std::vector<Obstacle> obstacles;
	std::vector<CarProto> cars;
	std::vector<LootProto> loot;
	std::vector<CreepProto> creeps;
	std::vector<WeaponProto> weapons;
	std::vector<ForceProto> forces;
	std::vector<FormationProto> formations;
	std::vector<std::vector<ObjectiveProto>> objectives;


	void load(json &source);
};