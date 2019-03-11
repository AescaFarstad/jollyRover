#include <CreepProto.h>

void from_json(const json &j, CreepProto &creep)
{
	creep.id = j.at("id");
	creep.name = j.at("name");
	creep.maxHealth = j.at("maxHealth");
	creep.speed = j.at("speed");
	creep.loot = j.at("loot");
	creep.cost = j.at("cost");
	creep.weapon = j.at("weapon");
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