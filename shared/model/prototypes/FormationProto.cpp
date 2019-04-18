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
	
	std::string strType = j.at("type");	
	if (strType == "assault")
		formation.type = FORMATION_TYPE::ASSAULT;
	else if (strType == "hold")
		formation.type = FORMATION_TYPE::HOLD;
	else if (strType == "transport")
		formation.type = FORMATION_TYPE::TRANSPORT;
	else
		THROW_FATAL_ERROR("josn parse error");
		
	formation.slots = j.at("slots").get<std::vector<FormationSlotProto>>();
	
	for(size_t i = 0; i < formation.slots.size(); i++)
	{
		formation.slots[i].index = i;
	}
	auto slots = formation.slots;
	for(size_t i = 0; i < formation.slots.size(); i++)
	{
		std::sort(slots.begin(), slots.end(), 
			[slot = formation.slots[i]](FormationSlotProto& a, FormationSlotProto& b)
			{
				if (a.index == slot.index)
					return true;
				if (b.index == slot.index)
					return false;
				if (a.optional != b.optional && !b.optional)
					return true;
				if (a.optional != b.optional && b.optional)
					return false;
				return b.offset.distanceTo(slot.offset) < a.offset.distanceTo(slot.offset);
			});
		std::string gg = "";
		for(auto& g : slots)
			gg += " " + std::to_string(g.index);	
		std::cout << "sort: " << formation.slots[i].index << " res: " << gg << "\n";
		
			
		for(size_t j = 0; j < FormationProto::CONNECTIONS; j++)
		{
			formation.slots[i].connections.push_back(FormationSlotConnectionProto{slots[j].index, formation.slots[i].offset - slots[j].offset});
		}
	}
}