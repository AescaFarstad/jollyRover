#pragma once

#include <json.hpp>
using json = nlohmann::json;

class LootProto
{
public:
	LootProto();
	~LootProto();

private:

};


void from_json(const json& j, LootProto& car);