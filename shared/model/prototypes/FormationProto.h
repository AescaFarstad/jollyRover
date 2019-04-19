#pragma once
#include <json.hpp>
#include <Point.h>
using json = nlohmann::json;

enum class FORMATION_TYPE : int32_t
{
	ASSAULT,
	HOLD,
	TRANSPORT
};


class FormationSlotConnectionProto
{
public:
	FormationSlotConnectionProto() = default;
	~FormationSlotConnectionProto() = default;
	
	int32_t slot;
	Point offset;	
};

class FormationSlotProto
{
public:
	FormationSlotProto() = default;
	~FormationSlotProto() = default;
	
	int32_t index;
	Point offset;
	int16_t creepType;
	bool optional;
	std::vector<FormationSlotConnectionProto> connections;
	
};

class FormationProto
{
public:
	FormationProto() = default;
	~FormationProto() = default;
	
	int16_t id;
	FORMATION_TYPE type;
	std::vector<FormationSlotProto> slots;
	int32_t strength;
	Point AA;
	Point BB;
};

void from_json(const json &j, FormationSlotProto &slot);
void from_json(const json &j, FormationProto &formation);