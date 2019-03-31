#include <ObjectiveProto.h>



void from_json(const json &j, ObjectiveProto &objective)
{
	objective.id = j.at("id");
	objective.forces = j.at("forces").get<std::vector<int16_t>>();
	objective.location = j.at("location");
	
	std::string strType = j.at("type");	
	if (strType == "oil")
		objective.type = OBJECTIVE_TYPE::OIL;
	else if (strType == "point")
		objective.type = OBJECTIVE_TYPE::POINT;
	else
		THROW_FATAL_ERROR("josn parse error");
		
	objective.weight = j.at("weight");
}