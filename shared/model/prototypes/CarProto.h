#pragma once
#include <json.hpp>
using json = nlohmann::json;

class CarProto
{
public:
	CarProto();
	~CarProto();

	int16_t id;
	int32_t maxHealth;
	float speed;

private:

};

void from_json(const json &j, CarProto &car);