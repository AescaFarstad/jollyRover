#include <CreepProto.h>
#include <CreepState.h>

void from_json(const json &j, CreepProto &creep)
{
	creep.id = j.at("id");
	creep.name = j.at("name");
	creep.maxHealth = j.at("maxHealth");
	creep.speed = j.at("speed");
	creep.speed /= 1000;
	creep.weight = j.at("weight");
	creep.size = j.at("size");
	creep.whiskers = j.at("whiskers");
	creep.maxWhiskerLength = j.at("maxWhiskerLength");
	
	if (creep.whiskers > CreepState::MAX_WHISKER_COUNT - 1)
		THROW_FATAL_ERROR("Too manu whiskers!");
	
	creep.loot = j.at("loot");
	creep.strength = j.at("strength");
	creep.weapon = j.at("weapon");
	
	std::string moveType = j.at("moveType");
	if (moveType == "tractor")	
		creep.moveType = MOVE_TYPE::TRACTOR;
	else if (moveType == "walker")	
		creep.moveType = MOVE_TYPE::WALKER;
	else
		THROW_FATAL_ERROR("Unknown creep move type: " + moveType);
	
	if (creep.moveType == MOVE_TYPE::TRACTOR)
	{
		creep.angularSpeed = j.at("angularSpeed");
		creep.acceleration.x = j.at("acceleration").at("0");
		creep.acceleration.y = j.at("acceleration").at("1");
		creep.normalFriction = j.at("normalFriction");
		creep.breaksStrength = j.at("breaksStrength");
		creep.omniDirectionalSpeed = j.at("omniDirectionalSpeed");
	}
	else
	{
		creep.normalFriction = 0;
	}
}

void from_json(const json &j, WeaponProto &weapon)
{
	weapon.id = j.at("id");
	weapon.damage = j.at("damage");
	weapon.attackSpeed = j.at("attackSpeed");
	weapon.splash = j.at("splash");
	weapon.range = j.at("range");
	weapon.bulletSpeed = j.at("bulletSpeed");
}