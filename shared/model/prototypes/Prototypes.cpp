#include <Prototypes.h>

void Prototypes::load(json &source)
{
	variables.load(source.at("variables"));
	obstacles = source.at("obstacles").get<std::vector<Obstacle>>();
	cars = source.at("cars").get<std::vector<CarProto>>();
	
	creeps = source.at("creeps").get<std::vector<CreepProto>>();
	weapons = source.at("weapons").get<std::vector<WeaponProto>>();
	forces = source.at("forces").get<std::vector<ForceProto>>();
	
	auto formationLayout = source.at("formationLayout").get<std::vector<FormationLayout>>();
	auto formationDef = source.at("formationDef").get<std::vector<FormationDef>>();	
	formations = FormationProto::createFormations(formationLayout, formationDef);
	
	auto allObjectives = source.at("objectives").get<std::vector<ObjectiveProto>>();
	
	moveObjectives.resize(forces.size());
	retreatObjectives.resize(forces.size());
	
	for(auto& o : allObjectives)
	{
		auto* obj = o.type == OBJECTIVE_TYPE::MOVE ? &moveObjectives : &retreatObjectives;
		
		for(auto& force : o.forces)
		{
			(*obj)[force].push_back(o);
		}
	}
	
	postProcess();
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
		formation.width = formation.BB.y - formation.AA.y;
		
		Point circleCenter = (formation.AA + formation.BB) / 2;
		float radius = (formation.BB - circleCenter).getLength();		
		for(auto& slot : formation.slots)
		{
			slot.priority = radius - slot.offset.getLength();
		}
	}
	
	variables.maxCreepSize = 0;
	for(auto& creep : creeps)
	{
		if (variables.maxCreepSize < creep.size)
			variables.maxCreepSize = creep.size;
	}
	
	variables.maxCarSize = 0;
	for(auto& car : cars)
	{
		if (variables.maxCarSize < car.size)
			variables.maxCarSize = car.size;
	}
	
	int32_t padding = 100;
	Point from(-padding, -padding);
	Point to(variables.fieldWidth + padding, variables.fieldHeight + padding);
	
	obstacleMap = SpatialMap<Obstacle>(10, false, from, to);
	obstacleMap.reset(obstacles.size() * 4);
	obstacleMap.addNonUnique(obstacles);
	
	
	
	for(auto& formation : formations)
	{
		formation.maxAngularSpeed = M_PI;
		
		auto slots = formation.slots;
		for(size_t i = 0; i < formation.slots.size(); i++)
		{
			std::sort(slots.begin(), slots.end(), 
				[slot = formation.slots[i], leader = formation.leader](FormationSlotProto& a, FormationSlotProto& b)
				{					
					if (a.index == slot.index)
						return false;
					if (b.index == slot.index)
						return true;
					
					if (leader >= 0 && b.index == leader)
						return false;
					if (leader >= 0 && a.index == leader)
						return true;
						
					if (a.optional != b.optional && !b.optional)
						return false;
					if (a.optional != b.optional && b.optional)
						return true;
					/*
					if (a.creepType != b.creepType)
						return b.creepType != slot.creepType;*/
							
					return b.offset.distanceTo(slot.offset) > a.offset.distanceTo(slot.offset);
				});
				
			for(int32_t j = 0; j < formation.connections && j < (int32_t)slots.size(); j++)
			{
				if (j == formation.leader || creeps[slots[j].creepType].weight >= creeps[formation.slots[i].creepType].weight)
					formation.slots[i].connections.push_back(FormationSlotConnectionProto{slots[j].index, formation.slots[i].offset - slots[j].offset});
			}
			
			// maxAngularSpeed
			float slotAngularSpeed = creeps[formation.slots[i].creepType].speed / formation.slots[i].offset.getLength();
			formation.maxAngularSpeed = std::min(formation.maxAngularSpeed, slotAngularSpeed);
		}
	}
	
	//formations.erase(std::remove_if(formations.begin(), formations.end(), [](FormationProto& f){return !f.enabled;}), formations.end());
	
	
	
}
