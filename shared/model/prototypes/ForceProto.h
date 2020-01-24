#include <json.hpp>
#include <Edge.h>

class ForceProto
{
public:
	
	int16_t id;
	Point spawnAA;
	Point spawnBB;
	int32_t freq[2];
	bool alwaysOn;

};

void from_json(const nlohmann::json& j, ForceProto& force);