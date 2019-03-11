#pragma once
#include <json.hpp>
using json = nlohmann::json;

class WeaponProto
{
public:
	WeaponProto() = default;
	~WeaponProto() = default;
	
	int16_t id;
	int32_t damage;
	int32_t attackSpeed;
	int32_t splash;
	int32_t range;
	int32_t bulletSpeed;

private:

};

class CreepProto
{
public:
	CreepProto() = default;
	~CreepProto() = default;

	int16_t id;
	std::string name;
	int32_t maxHealth;
	float speed;
	int32_t loot;
	int32_t cost;
	int16_t weapon;

private:

};

void from_json(const json &j, CreepProto &creep);
void from_json(const json &j, WeaponProto &weapon);