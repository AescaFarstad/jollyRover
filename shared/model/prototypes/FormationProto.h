#include <json.hpp>
#include <Point.h>
using json = nlohmann::json;

enum FORMATION_TYPE
{
	ASSAULT,
	HOLD,
	TRANSPORT
};

class FormationSlotProto
{
public:
	FormationSlotProto() = default;
	~FormationSlotProto() = default;
	
	Point offset;
	int16_t creepType;
	bool optional;
	
};

class FormationProto
{
public:
	FormationProto() = default;
	~FormationProto() = default;
	
	int16_t id;
	FORMATION_TYPE type;
	std::vector<FormationSlotProto> slots;
};

void from_json(const json &j, FormationSlotProto &slot);
void from_json(const json &j, FormationProto &formation);