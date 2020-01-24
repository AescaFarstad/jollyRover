#pragma once
#include <json.hpp>
#include <Point.h>

enum class FORMATION_TYPE : int32_t
{
	ASSAULT,
	HOLD,
	TRANSPORT
};


class FormationSlotConnectionProto
{
public:	
	int32_t slot;
	Point offset;	
};

class FormationSlotProto
{
public:	
	int32_t index;
	Point offset;
	int16_t creepType;
	///distance to the circle surrounding the bounding box of the formation
	int32_t priority; 
	bool optional;
	std::vector<FormationSlotConnectionProto> connections;
	
};
void from_json(const nlohmann::json& j, FormationSlotProto& slot);

class FormationLayout
{
public:
	int16_t id;
	std::vector<FormationSlotProto> slots;
	int32_t leader;
	int32_t connections;
};

void from_json(const nlohmann::json& j, FormationLayout& layout);

class FormationDef
{
public:
	int16_t id;
	int16_t layout;
	FORMATION_TYPE type;
	float relativeAgroability;
	float finalRotationModifier;
	float moveSpeedModifier;
	bool enabled;
	std::vector<int16_t> forces;
	std::vector<int32_t> slots;
};

void from_json(const nlohmann::json& j, FormationDef& def);

class FormationProto
{
public:

	static std::vector<FormationProto> createFormations(std::vector<FormationLayout>& layouts, std::vector<FormationDef>& definitions);
	
	int16_t id;
	bool enabled;
	FORMATION_TYPE type;
	std::vector<int16_t> forces;
	std::vector<FormationSlotProto> slots;
	int32_t strength;
	int32_t width;
	int32_t leader;
	int32_t connections;
	float maxAngularSpeed;
	Point AA;
	Point BB;
	float relativeAgroability;
	float finalRotationModifier;
	float moveSpeedModifier;
};
