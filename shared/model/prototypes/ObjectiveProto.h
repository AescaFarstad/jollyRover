#pragma once
#include <json.hpp>
#include <Point.h>

enum class OBJECTIVE_TYPE : int8_t
{
	MOVE,
	RETREAT
};

class ObjectiveProto
{
public:	
	int16_t id;
	OBJECTIVE_TYPE type;
	Point location;
	std::vector<int16_t> forces;
	int32_t weight;
};

void from_json(const nlohmann::json& j, ObjectiveProto& objective);
