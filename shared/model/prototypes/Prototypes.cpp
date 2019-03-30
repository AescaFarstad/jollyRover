#include <Prototypes.h>

Prototypes::Prototypes()
{
}

Prototypes::~Prototypes()
{
}


void Prototypes::load(json &source)
{
	variables.load(source.at("variables"));
	obstacles = source.at("obstacles").get<std::vector<Obstacle>>();
	cars = source.at("cars").get<std::vector<CarProto>>();
	
	creeps = source.at("creeps").get<std::vector<CreepProto>>();
	weapons = source.at("weapons").get<std::vector<WeaponProto>>();
	forces = source.at("forces").get<std::vector<ForceProto>>();
	
	formations = source.at("formations").get<std::vector<FormationProto>>();
	auto allObjectives = source.at("objectives").get<std::vector<ObjectiveProto>>();
	
	for(auto& force : forces)
		objectives.emplace_back();
	
	for(auto& o : allObjectives)
	{
		for(auto& force : o.forces)
		{
			objectives[force].push_back(o);
		}
	}
	
	postProcess();

	//cars = source.at("loot").get<std::vector<LootProto>>();
	//loot = source.at("cars").get<std::vector<CarProto>>();
}

void Prototypes::postProcess()
{
	for(auto& formation : formations)
	{
		formation.strength = 0;
		for(auto& slot : formation.slots)
		{
			formation.strength += creeps[slot.creepType].strength;
		}
	}
	
	variables.maxCreepSize = 0;
	for(auto& creep : creeps)
	{
		if (variables.maxCreepSize < creep.size)
			variables.maxCreepSize = creep.size;
	}
}
