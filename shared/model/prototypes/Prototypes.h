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
#include <ForceProto.h>
#include <SpatialMap.h>


class Prototypes
{
public:
	VariableProto variables;
	std::vector<Obstacle> obstacles;
	std::vector<CarProto> cars;
	std::vector<LootProto> loot;
	std::vector<CreepProto> creeps;
	std::vector<WeaponProto> weapons;
	std::vector<ForceProto> forces;
	std::vector<FormationProto> formations;
	std::vector<std::vector<ObjectiveProto>> moveObjectives;
	std::vector<std::vector<ObjectiveProto>> retreatObjectives;
	SpatialMap<Obstacle> obstacleMap;
	
	void postProcess();

	void load(nlohmann::json& source);
};