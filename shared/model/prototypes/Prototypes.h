#pragma once

#include <json.hpp>
#include <vector>
#include <VariableProto.h>
#include <Obstacle.h>
#include <CarProto.h>
#include <LootProto.h>

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

	const CarProto* getCar(int16_t id);
	const LootProto* getLoot(int16_t id);

	void load(json &source);
};