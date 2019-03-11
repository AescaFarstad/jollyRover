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
		formation.type = ASSAULT;
	else if (strType == "hold")
		formation.type = HOLD;
	else if (strType == "transport")
		formation.type = TRANSPORT;
	else
		THROW_FATAL_ERROR("josn parse error");
		
	formation.slots = j.at("slots").get<std::vector<FormationSlotProto>>();
}