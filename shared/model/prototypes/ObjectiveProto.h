
#include <json.hpp>
#include <Point.h>
using json = nlohmann::json;

enum class OBJECTIVE_TYPE : int8_t
{
	OIL,
	POINT
};

class ObjectiveProto
{
public:
	ObjectiveProto() = default;
	~ObjectiveProto() = default;
	
	int16_t id;
	OBJECTIVE_TYPE type;
	Point location;
	std::vector<int16_t> forces;
	int32_t weight;
	
};

void from_json(const json &j, ObjectiveProto &objective);
