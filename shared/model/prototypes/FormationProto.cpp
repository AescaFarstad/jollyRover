#include <FormationProto.h>


void from_json(const json &j, FormationSlotProto &slot)
{
	slot.offset = j.at("offset");
	slot.creepType = -1;
	//slot.creepType = j.find("creepType") == j.end() ? -1 : j.at("creepType");
	slot.optional = j.find("optional") == j.end() ? false : (bool)j.at("optional");
}

void from_json(const json &j, FormationLayout &layout)
{
	layout.id = j.at("id");	
	layout.leader = j.at("leader");
	layout.connections = j.at("connections");
	layout.slots = j.at("slots").get<std::vector<FormationSlotProto>>();	
}

void from_json(const json &j, FormationDef &def)
{
	def.id = j.at("id");	
	def.layout = j.at("layout");
	
	std::string strType = j.at("type");	
	if (strType == "assault")
		def.type = FORMATION_TYPE::ASSAULT;
	else if (strType == "hold")
		def.type = FORMATION_TYPE::HOLD;
	else if (strType == "transport")
		def.type = FORMATION_TYPE::TRANSPORT;
	else
		THROW_FATAL_ERROR("josn parse error");
		
	def.relativeAgroability = j.at("relativeAgroability");
	def.enabled = j.at("enabled");	
	
	def.forces = j.at("forces").get<std::vector<int16_t>>();
	def.slots = j.at("slots").get<std::vector<int32_t>>();		
}

std::vector<FormationProto> FormationProto::createFormations(std::vector<FormationLayout>& layouts, std::vector<FormationDef>& definitions)
{
	std::vector<FormationProto> result;	
	
	for(auto& def : definitions)
	{		
		auto layout =  std::find_if(layouts.begin(), layouts.end(), [&def](FormationLayout& l){ return l.id == def.layout;});
		if (layout == layouts.end())
			THROW_FATAL_ERROR("No formation layout exists for the definition");
			
		if (layout->slots.size() != def.slots.size())
			THROW_FATAL_ERROR("Formation def and layout slots are not the same size");
			
		auto& form = result.emplace_back();
		
		form.id = def.id;
		form.enabled = def.enabled;
		form.type = def.type;
		form.forces = def.forces;
		form.slots = layout->slots;
		form.leader = layout->leader;
		form.connections = layout->connections;
		form.relativeAgroability = def.relativeAgroability;
		
		for(size_t i = 0; i < def.slots.size(); i++)
		{
			form.slots[i].index = i;
			form.slots[i].creepType = def.slots[i];
		}
	}
	
	return result;
}