#include <json.hpp>
#include <Edge.h>

class ForceProto
{
public:
	ForceProto() = default;
	~ForceProto() = default;
	
	
	int16_t id;
	Point spawnAA;
	Point spawnBB;
	int32_t freq[2];
	bool alwaysOn;

};

void from_json(const json &j, ForceProto &force);