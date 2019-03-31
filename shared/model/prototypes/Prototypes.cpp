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
	
	objectives.resize(forces.size());
	
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
		
		formation.AA.x = FMath::F_MAX;
		formation.AA.y = FMath::F_MAX;
		formation.BB.x = FMath::F_MIN;
		formation.BB.y = FMath::F_MIN;
		
		for(auto& slot : formation.slots)
		{
			auto& creep = this->creeps[slot.creepType];
			
			formation.AA.x = std::min(formation.AA.x, slot.offset.x - creep.size);
			formation.AA.y = std::min(formation.AA.y, slot.offset.y - creep.size);
			formation.BB.x = std::max(formation.BB.x, slot.offset.x + creep.size);
			formation.BB.y = std::max(formation.BB.y, slot.offset.y + creep.size);
		}
	}
	
	variables.maxCreepSize = 0;
	for(auto& creep : creeps)
	{
		if (variables.maxCreepSize < creep.size)
			variables.maxCreepSize = creep.size;
	}
	
	int32_t padding = 100;
	Point from(-padding, -padding);
	Point to(variables.fieldWidth + padding, variables.fieldHeight + padding);
	
	obstacleMap = SpatialMap<Obstacle>(10, false, from, to);
	obstacleMap.setNonUnique(obstacles);
	
	
	
	
	
}
