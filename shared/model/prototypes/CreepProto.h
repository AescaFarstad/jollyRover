#pragma once
#include <Point.h>
#include <json.hpp>
using json = nlohmann::json;

enum class MOVE_TYPE : uint8_t
{
	WALKER,
	TRACTOR
};

class TextureDef;
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
	std::vector<std::string> projectileTextureName;
	std::vector<TextureDef*> projectileTexture;

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
	int32_t strength;
	int16_t weapon;
	int32_t weight;
	float slowOnRollOver;
	bool rolloverable;
	int16_t size;
	int8_t whiskers;
	int16_t maxWhiskerLength;
	MOVE_TYPE moveType;
	float angularSpeed;
	Point acceleration; //x - at 0 speed, y - at max speed
	float breaksStrength;
	float omniDirectionalSpeed;
	float normalFriction;
	std::vector<std::string> hullTextureName;
	std::vector<std::string> gunTextureName;
	
	std::vector<TextureDef*> hullTexture;
	std::vector<TextureDef*> gunTexture;

private:

};

void from_json(const json &j, CreepProto &creep);
void from_json(const json &j, WeaponProto &weapon);