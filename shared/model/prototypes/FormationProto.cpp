#include <FormationProto.h>


void from_json(const json &j, FormationSlotProto &slot)
{
	slot.offset = j.at("offset");
	slot.creepType = j.at("creepType");
	slot.optional = j.at("optional");
}


void from_json(const json &j, FormationProto &formation)
{
	formation.id = j.at("id");
	formation.enabled = j.at("enabled");
	
	std::string strType = j.at("type");	
	if (strType == "assault")
		formation.type = FORMATION_TYPE::ASSAULT;
	else if (strType == "hold")
		formation.type = FORMATION_TYPE::HOLD;
	else if (strType == "transport")
		formation.type = FORMATION_TYPE::TRANSPORT;
	else
		THROW_FATAL_ERROR("josn parse error");
		
	formation.forces = j.at("forces").get<std::vector<int16_t>>();
	formation.slots = j.at("slots").get<std::vector<FormationSlotProto>>();
	
	for(size_t i = 0; i < formation.slots.size(); i++)
	{
		formation.slots[i].index = i;
	}
	
	formation.leader = j.at("leader");
	formation.connections = j.at("connections");
	formation.relativeAgroability = j.at("relativeAgroability");
}